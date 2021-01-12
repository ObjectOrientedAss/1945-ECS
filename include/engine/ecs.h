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

//the struct containing all the entities allocated and to deallocate, and all the systems allocated and to deallocate, right now
struct EntityComponentSystem
{
    aiv_vector *__entitiesToDestroy;   //add an existing entity here when it is marked to be destroyed, remove it when it is destroyed
    aiv_vector *__componentsToDestroy; //add an existing component here when it is marked to be destroyed, remove it when it is destroyed
    aiv_vector *__entities;            //add an entity here when it is created, remove it when it is destroyed
    aiv_vector *__systems;             //add a component to the right index in here when it is added to an entity, remove it when it or the owning entity are destroyed
};

//allocate and start the ECS
struct EntityComponentSystem *__ECSInit();
//stop and deallocate the ECS
void __DestroyECS(struct EntityComponentSystem *ECS);
//clear the ECS and make a fresh one, with all the structures clear and ready to start over. You should call it on every scene change.
struct EntityComponentSystem* ECSReset(struct EntityComponentSystem* ECS);
//create an entity with the given informations
struct Entity *CreateEntity(EntityType type, EntityFamily family, boolean activateImmediately, struct EntityComponentSystem *ECS);
//add a component on the given entity with the given informations
struct Component *AddComponent(struct Entity *entity, ComponentType type, void (*behaviour)(struct Component *selfComponent, struct Game* game), struct EntityComponentSystem* ECS);
//get a component of the requested type on the given entity. Beware, the first component of this type found will be returned (or NULL)
struct Component *GetComponent(struct Entity *entity, ComponentType type);
//get the struct Component* -> void* data of the requested type on the given entity. Beware, the first component of this type found will be returned (or NULL)
void *GetComponentData(struct Entity *entity, ComponentType type);
//get the system vector of the given components type from the ECS. Do not modify the vector itself or anything related.
aiv_vector* GetSystem(struct EntityComponentSystem* ECS, ComponentType system);
//mark a component to be immediately deactivated and destroyed at the end of the current frame
void MarkComponentToDestroy(struct Component *component, struct EntityComponentSystem* ECS);
//mark an entity to be immediately deactivated and destroyed at the end of the current frame
void MarkEntityToDestroy(struct Entity *entity, struct EntityComponentSystem* ECS);
//destroy all the components and the entities, marked to be destroyed during the current frame. You should call it at the very end of the frame.
void Collect(struct EntityComponentSystem *ECS);
//destroy a single component
void __DestroyComponent(struct Component *component, struct EntityComponentSystem* ECS);
//destroy all the components contained in the given vector
void __DestroyComponents(aiv_vector *componentsContainer, struct EntityComponentSystem* ECS);
//destroy a single entity
void __DestroyEntity(struct Entity *entity, struct EntityComponentSystem* ECS);
//destroy all the entities contained in the given vector
void __DestroyEntities(aiv_vector *entitiesContainer, struct EntityComponentSystem* ECS);
//add a component to its ECS system. It is added only if its ComponentType value is >= 0
void __AddToSystems(struct Component *component, struct EntityComponentSystem* ECS);
//remove a component from its ECS system. It is removed only if its ComponentType value is < 0
void __RemoveFromSystems(struct Component *component, struct EntityComponentSystem* ECS);
//add an entity to the ECS
void __RegisterEntity(struct Entity *entity, struct EntityComponentSystem* ECS);
//remove an entity from the ECS
void __UnregisterEntity(struct Entity *entity, struct EntityComponentSystem* ECS);
//activate/deactivate an entity
void SetEntityActiveStatus(struct Entity* entity, boolean active);
//activate/deactivate a component
void SetComponentActiveStatus(struct Component* component, boolean active);

#endif //ECS_H