#ifndef ECS_H
#define ECS_H
#include <stdlib.h>
#include "aiv-vector.h"
#include "vec2.h"
#include "graphics_engine.h"
#include "audio_engine.h"
#include "game.h"

#include "component.h"
#include "entity.h"

//ALL EXISTING COMPONENTS:
struct EntityComponentSystem
{
    aiv_vector *entitiesToDestroy;   //add an existing entity here when it is marked to be destroyed, remove it when it is destroyed
    aiv_vector *componentsToDestroy; //add an existing component here when it is marked to be destroyed, remove it when it is destroyed
    aiv_vector *entities;            //add an entity here when it is created, remove it when it is destroyed
    aiv_vector *systems;             //add a component to the right index in here when it is added to an entity, remove it when it or the owning entity are destroyed
};

//-----END OF DATA SYSTEMS-----//

struct EntityComponentSystem *ECSInit();
void DestroyECS(struct EntityComponentSystem *ECS);
struct EntityComponentSystem* ECSReset(struct EntityComponentSystem* ECS);
struct Entity *CreateEntity(EntityType type, EntityFamily family, boolean activateImmediately, struct EntityComponentSystem *ECS);
struct Component *AddComponent(struct Entity *entity, ComponentType type, void (*behaviour)(struct Component *selfComponent, struct Game* game));
struct Component *GetComponent(struct Entity *entity, ComponentType type);
void *GetComponentData(struct Entity *entity, ComponentType type);
void MarkComponentToDestroy(struct Component *component);
void MarkEntityToDestroy(struct Entity *entity);
void DestroyComponent(struct Component *component);
void DestroyComponents(aiv_vector *componentsContainer);
void DestroyEntity(struct Entity *entity);
void DestroyEntities(aiv_vector *entitiesContainer);
void AddToSystems(struct Component *component);
void RemoveFromSystems(struct Component *component);
void RegisterEntity(struct Entity *entity);
void UnregisterEntity(struct Entity *entity);
void SetEntityActiveStatus(struct Entity* entity, boolean active);
void SetComponentActiveStatus(struct Component* component, boolean active);

#endif //ECS_H