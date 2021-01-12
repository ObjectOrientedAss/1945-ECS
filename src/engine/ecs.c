#include "ecs.h"

struct EntityComponentSystem *__ECSInit()
{
    struct EntityComponentSystem *ECS = calloc(1, sizeof(struct EntityComponentSystem));
    //CREATE THE ENTITIES/SYSTEM VECTORS!
    ECS->__entities = aiv_vector_new_with_cap(10);
    ECS->__entitiesToDestroy = aiv_vector_new_with_cap(10);
    ECS->__componentsToDestroy = aiv_vector_new_with_cap(10);
    ECS->__systems = aiv_vector_new_with_cap((uint)c_type_last);

    for (int i = 0; i < (int)c_type_last; i++)
    {
        aiv_vector *currentSystem = aiv_vector_new_with_cap(1);
        aiv_vector_add(ECS->__systems, currentSystem);
    }

    return ECS;
}

struct EntityComponentSystem *ECSReset(struct EntityComponentSystem *ECS)
{
    __DestroyECS(ECS);
    return __ECSInit();
}

void __DestroyECS(struct EntityComponentSystem *ECS)
{
    //DestroyComponents(ECS->componentsToDestroy);
    aiv_vector_destroy(ECS->__componentsToDestroy);

    //DestroyEntities(ECS->entitiesToDestroy);
    aiv_vector_destroy(ECS->__entitiesToDestroy);

    __DestroyEntities(ECS->__entities, ECS);
    aiv_vector_destroy(ECS->__entities);

    for (int i = 0; i < (int)c_type_last; i++)
    {
        aiv_vector_destroy(aiv_vector_at(ECS->__systems, i));
    }

    aiv_vector_destroy(ECS->__systems);
    free(ECS);
}

struct Entity *CreateEntity(EntityType type, EntityFamily family, boolean activateImmediately, struct EntityComponentSystem *ECS)
{
    struct Entity *entity = calloc(1, sizeof(struct Entity));
    entity->__components = aiv_vector_new_with_cap(1);
    entity->type = type;
    entity->family = family;
    entity->__markedToDestroy = false;
    entity->active = activateImmediately;
    __RegisterEntity(entity, ECS);

    return entity;
}

struct Component *AddComponent(struct Entity *entity, ComponentType componentType, void (*behaviour)(struct Component *selfComponent, struct Game *game), struct EntityComponentSystem* ECS)
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
        component->__markedToDestroy = false;
        component->active = true;
        component->behaviour = behaviour;
        aiv_vector_add(entity->__components, component);
        __AddToSystems(component, ECS);
        return component;
    }

    free(component);
    return NULL;
}

struct Component *GetComponent(struct Entity *entity, ComponentType type)
{
    for (uint i = 0; i < aiv_vector_size(entity->__components); i++)
    {
        struct Component *c = aiv_vector_at(entity->__components, i);
        if (c->type == type)
            return c;
    }

    return NULL;
}

void *GetComponentData(struct Entity *entity, ComponentType type)
{
    return GetComponent(entity, type)->data;
}

void MarkComponentToDestroy(struct Component *component, struct EntityComponentSystem* ECS)
{
    component->active = false;
    component->__markedToDestroy = true;
    for (uint i = 0; i < aiv_vector_size(component->owner->__components); i++)
    {
        if (component == aiv_vector_at(component->owner->__components, i))
            aiv_vector_remove_at(component->owner->__components, i);
    }

    aiv_vector_add(ECS->__componentsToDestroy, component);
}

void MarkEntityToDestroy(struct Entity *entity, struct EntityComponentSystem* ECS)
{
    entity->active = false;
    entity->__markedToDestroy = true;

    for (uint i = 0; i < aiv_vector_size(entity->__components); i++)
    {
        //just deactivate the components, no need to mark them to be destroyed: when the entity gets destroyed it is also destroying the components!
        struct Component *c = aiv_vector_at(entity->__components, i);
        c->active = false;
    }

    aiv_vector_add(ECS->__entitiesToDestroy, entity);
}

void __DestroyComponent(struct Component *component, struct EntityComponentSystem* ECS)
{
    //THIS IS TOTALLY UNOPTIMIZED! REMOVE ALL AT ONCE!
    __RemoveFromSystems(component, ECS);
    //---

    free(component->data);
    free(component);
}

void __DestroyEntity(struct Entity *entity, struct EntityComponentSystem* ECS)
{
    __UnregisterEntity(entity, ECS);
    for (int i = aiv_vector_size(entity->__components) - 1; i >= 0; i--)
    {
        struct Component *component = aiv_vector_remove_at(entity->__components, i);
        __DestroyComponent(component, ECS);
    }
    aiv_vector_destroy(entity->__components);
    free(entity);
}

void Collect(struct EntityComponentSystem *ECS)
{
    __DestroyComponents(ECS->__componentsToDestroy, ECS);
    __DestroyEntities(ECS->__entitiesToDestroy, ECS);
}

void __DestroyComponents(aiv_vector *componentsContainer, struct EntityComponentSystem* ECS)
{
    for (int i = aiv_vector_size(componentsContainer) - 1; i >= 0; i--)
    {
        struct Component *component = aiv_vector_remove_at(componentsContainer, i);
        __DestroyComponent(component, ECS);
    }
}

void __DestroyEntities(aiv_vector *entitiesContainer, struct EntityComponentSystem* ECS)
{
    for (int i = aiv_vector_size(entitiesContainer) - 1; i >= 0; i--)
    {
        struct Entity *entity = aiv_vector_at(entitiesContainer, i);
        __DestroyEntity(entity, ECS);
    }
}

void __AddToSystems(struct Component *component, struct EntityComponentSystem* ECS)
{
    if (component->type >= 0) //if index is negative, it is not inside a system
    {
        aiv_vector *system = aiv_vector_at(ECS->__systems, (uint)component->type);
        component->__systemIndex = aiv_vector_size(system);
        aiv_vector_add(system, component);
    }
}

void __RemoveFromSystems(struct Component *component, struct EntityComponentSystem* ECS)
{
    if (component->type >= 0) //if index is negative, it is not inside a system.
    {
        aiv_vector *system = aiv_vector_at(ECS->__systems, (uint)component->type);
        aiv_vector_remove_at(system, component->__systemIndex);
        for (uint i = component->__systemIndex; i < aiv_vector_size(system); i++)
        {
            struct Component *currComp = aiv_vector_at(system, i);
            currComp->__systemIndex--;
        }
    }
}

void __RegisterEntity(struct Entity *entity, struct EntityComponentSystem* ECS)
{
    entity->__entityIndex = aiv_vector_size(ECS->__entities);
    aiv_vector_add(ECS->__entities, entity);
}

void __UnregisterEntity(struct Entity *entity, struct EntityComponentSystem* ECS)
{
    aiv_vector *entities = ECS->__entities;
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

aiv_vector* GetSystem(struct EntityComponentSystem* ECS, ComponentType system)
{
    return aiv_vector_at(ECS->__systems, system);
}