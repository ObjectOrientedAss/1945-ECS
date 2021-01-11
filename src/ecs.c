#include "ecs.h"

struct EntityComponentSystem *ECSInit()
{
    struct EntityComponentSystem *ECS = calloc(1, sizeof(struct EntityComponentSystem));
    //CREATE THE ENTITIES/SYSTEM VECTORS!
    //TODO:
    ECS->entities = aiv_vector_new_with_cap(10);
    ECS->entitiesToDestroy = aiv_vector_new_with_cap(10);
    ECS->componentsToDestroy = aiv_vector_new_with_cap(10);
    ECS->systems = aiv_vector_new_with_cap((uint)c_type_last);

    for (int i = 0; i < (int)c_type_last; i++)
    {
        aiv_vector *currentSystem = aiv_vector_new_with_cap(1);
        aiv_vector_add(ECS->systems, currentSystem);
    }

    return ECS;
}

struct EntityComponentSystem *ECSReset(struct EntityComponentSystem *ECS)
{
    DestroyECS(ECS);
    return ECSInit();
}

void DestroyECS(struct EntityComponentSystem *ECS)
{
    //DestroyComponents(ECS->componentsToDestroy);
    aiv_vector_destroy(ECS->componentsToDestroy);

    //DestroyEntities(ECS->entitiesToDestroy);
    aiv_vector_destroy(ECS->entitiesToDestroy);

    DestroyEntities(ECS->entities);
    aiv_vector_destroy(ECS->entities);

    for (int i = 0; i < (int)c_type_last; i++)
    {
        aiv_vector_destroy(aiv_vector_at(ECS->systems, i));
    }

    aiv_vector_destroy(ECS->systems);
    free(ECS);
}

struct Entity *CreateEntity(EntityType type, EntityFamily family, boolean activateImmediately, struct EntityComponentSystem *ECS)
{
    struct Entity *entity = calloc(1, sizeof(struct Entity));
    entity->components = aiv_vector_new_with_cap(1);
    entity->type = type;
    entity->family = family;
    entity->markedToDestroy = false;
    entity->active = activateImmediately;
    entity->ECS = ECS;
    RegisterEntity(entity);

    return entity;
}

//ADDS THE SPECIFIED COMPONENT TO THE ENTITY.
//TODO: remove game as param it is useless.
struct Component *AddComponent(struct Entity *entity, ComponentType componentType, void (*behaviour)(struct Component *selfComponent, struct Game *game))
{
    struct Component *component = (struct Component *)calloc(1, sizeof(struct Component));

    switch (componentType)
    {
    case TimedBehaviourC:
        struct TimedBehaviourComponent *timedBehaviourComponent = calloc(1, sizeof(struct TimedBehaviourComponent));
        if (timedBehaviourComponent == NULL)
            break;
        component->type = TimedBehaviourC;
        component->data = timedBehaviourComponent;
        break;

    case ShootC:
        struct ShootComponent *shootComponent = calloc(1, sizeof(struct ShootComponent));
        if (shootComponent == NULL)
            break;
        component->type = ShootC;
        component->data = shootComponent;
        break;

    case TransformC:
        struct TransformComponent *transformComponent = calloc(1, sizeof(struct TransformComponent));
        if (transformComponent == NULL)
            break;
        component->type = TransformC;
        component->data = transformComponent;
        break;

    case InputC:
        struct InputComponent *inputComponent = calloc(1, sizeof(struct InputComponent));
        if (inputComponent == NULL)
            break;
        component->type = InputC;
        component->data = inputComponent;
        break;

    case UIC:
        struct UIComponent *buttonComponent = calloc(1, sizeof(struct UIComponent));
        if (buttonComponent == NULL)
            break;
        component->type = UIC;
        component->data = buttonComponent;
        break;

    case MovementC:
        struct MovementComponent *movementComponent = calloc(1, sizeof(struct MovementComponent));
        if (movementComponent == NULL)
            break;
        component->type = MovementC;
        component->data = movementComponent;
        break;

    case PhysicsC:
        struct PhysicsComponent *physicsComponent = calloc(1, sizeof(struct PhysicsComponent));
        if (physicsComponent == NULL)
            break;
        component->type = PhysicsC;
        component->data = physicsComponent;
        break;

    case HealthC:
        struct HealthComponent *healthComponent = calloc(1, sizeof(struct HealthComponent));
        if (healthComponent == NULL)
            break;
        component->type = HealthC;
        component->data = healthComponent;
        break;

    case AnimatorC:
        struct AnimatorComponent *animatorComponent = calloc(1, sizeof(struct AnimatorComponent));
        if (animatorComponent == NULL)
            break;
        component->type = AnimatorC;
        component->data = animatorComponent;
        break;

    case AudioC:
        struct AudioComponent *audioComponent = calloc(1, sizeof(struct AudioComponent));
        if (audioComponent == NULL)
            break;
        component->type = AudioC;
        component->data = audioComponent;
        break;

    case RenderC:
        struct RenderComponent *renderComponent = calloc(1, sizeof(struct RenderComponent));
        if (renderComponent == NULL)
            break;
        component->type = RenderC;
        component->data = renderComponent;
        break;

    case TextC:
        struct TextComponent *textComponent = calloc(1, sizeof(struct TextComponent));
        if (textComponent == NULL)
            break;
        component->type = TextC;
        component->data = textComponent;
        break;
    }

    if (component->data != NULL)
    {
        component->owner = entity;
        component->markedToDestroy = false;
        component->active = true;
        component->behaviour = behaviour;
        aiv_vector_add(entity->components, component);
        AddToSystems(component);
        return component;
    }

    free(component);
    return NULL;
}

//GET A POINTER TO THE MAIN COMPONENT OF THE GIVEN TYPE FROM THE ENTITY COMPONENTS LIST.
//RETURNS NULL IF THE COMPONENT IS NOT FOUND.
struct Component *GetComponent(struct Entity *entity, ComponentType type)
{
    for (uint i = 0; i < entity->components->__count; i++)
    {
        struct Component *c = aiv_vector_at(entity->components, i);
        if (c->type == type)
            return c;
    }

    return NULL;
}

//GET A POINTER TO THE SPECIFIC COMPONENT DATA TYPE CONTAINING THE REAL DATA AND BEHAVIOURS OF THE COMPONENT ON THE ENTITY.
//RETURNS NULL IF THE COMPONENT IS NOT FOUND.
void *GetComponentData(struct Entity *entity, ComponentType type)
{
    return GetComponent(entity, type)->data;
}

//MARK THE GIVEN COMPONENT TO BE DESTROYED AT THE END OF THE FRAME.
//ALSO DEACTIVATES THE COMPONENT FROM NOW UNTIL THEN.
void MarkComponentToDestroy(struct Component *component)
{
    component->active = false;
    component->markedToDestroy = true;
    for (int i = 0; i < aiv_vector_size(component->owner->components); i++)
    {
        if (component == aiv_vector_at(component->owner->components, i))
            aiv_vector_remove_at(component->owner->components, i);
    }

    aiv_vector_add(component->owner->ECS->componentsToDestroy, component);
}

//MARK THE GIVEN ENTITY TO BE DESTROYED AT THE END OF THE FRAME.
//ALSO DEACTIVATES THE ENTITY FROM NOW UNTIL THEN.
void MarkEntityToDestroy(struct Entity *entity)
{
    entity->active = false;
    entity->markedToDestroy = true;

    for (uint i = 0; i < aiv_vector_size(entity->components); i++)
    {
        //just deactivate the components, no need to mark them to be destroyed: when the entity gets destroyed it is also destroying the components!
        struct Component *c = aiv_vector_at(entity->components, i);
        c->active = false;
    }

    aiv_vector_add(entity->ECS->entitiesToDestroy, entity);
}

void DestroyComponent(struct Component *component)
{
    //THIS IS TOTALLY UNOPTIMIZED! REMOVE ALL AT ONCE!
    RemoveFromSystems(component);
    //---

    free(component->data);
    free(component);
}

void DestroyEntity(struct Entity *entity)
{
    UnregisterEntity(entity);
    for (int i = entity->components->__count - 1; i >= 0; i--)
    {
        struct Component *component = aiv_vector_remove_at(entity->components, i);
        DestroyComponent(component);
    }
    aiv_vector_destroy(entity->components);
    free(entity);
}

void DestroyComponents(aiv_vector *componentsContainer)
{
    for (int i = componentsContainer->__count - 1; i >= 0; i--)
    {
        struct Component *component = aiv_vector_remove_at(componentsContainer, i);
        DestroyComponent(component);
    }
}

void DestroyEntities(aiv_vector *entitiesContainer)
{
    for (int i = entitiesContainer->__count - 1; i >= 0; i--)
    {
        struct Entity *entity = aiv_vector_at(entitiesContainer, i);
        DestroyEntity(entity);
    }
}

void AddToSystems(struct Component *component)
{
    if ((int)component->type >= 0) //if index is negative, it is not inside a system
    {
        aiv_vector *system = aiv_vector_at(component->owner->ECS->systems, (uint)component->type);
        component->__systemIndex = aiv_vector_size(system);
        aiv_vector_add(system, component);
    }
}

void RemoveFromSystems(struct Component *component)
{
    if ((int)component->type >= 0) //if index is negative, it is not inside a system.
    {
        aiv_vector *system = aiv_vector_at(component->owner->ECS->systems, (uint)component->type);
        aiv_vector_remove_at(system, component->__systemIndex);
        for (uint i = component->__systemIndex; i < aiv_vector_size(system); i++)
        {
            struct Component *currComp = aiv_vector_at(system, i);
            currComp->__systemIndex--;
        }
    }
}

void RegisterEntity(struct Entity *entity)
{
    entity->__entityIndex = aiv_vector_size(entity->ECS->entities);
    aiv_vector_add(entity->ECS->entities, entity);
}

void UnregisterEntity(struct Entity *entity)
{
    aiv_vector *entities = entity->ECS->entities;
    aiv_vector_remove_at(entities, entity->__entityIndex);
    for (uint i = entity->__entityIndex; i < aiv_vector_size(entities); i++)
    {
        struct Entity *currEnt = aiv_vector_at(entities, i);
        currEnt->__entityIndex--;
    }
}

void SetEntityActiveStatus(struct Entity *entity, boolean active)
{
    entity->active = active;
}

void SetComponentActiveStatus(struct Component *component, boolean active)
{
    component->active = active;
}