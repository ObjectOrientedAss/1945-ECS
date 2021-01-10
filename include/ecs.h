#ifndef ECS_H
#define ECS_H
#include <stdlib.h>
#include "aiv-vector.h"
#include "vec2.h"
#include "graphics_engine.h"
#include "audio_engine.h"
#include "game.h"

#include "entity.h"
#include "animator_component.h"
#include "audio_component.h"
#include "health_component.h"
#include "input_component.h"
#include "movement_component.h"
#include "physics_component.h"
#include "render_component.h"
#include "shoot_component.h"
#include "text_component.h"
#include "timed_behaviour_component.h"
#include "transform_component.h"
#include "ui_component.h"

//<0 value = Data System (Is not updated by the ECS loop)
//0+ value = ECS System (Is automatically updated every frame by the ECS loop)
enum c_type
{
    //------DATA SYSTEMS------//
    TextC = -3,
    AudioC,
    TransformC,

    //^^^^^^DATA SYSTEMS^^^^^^//
    //------------------------//
    //------ECS  SYSTEMS------//

    TimedBehaviourC,
    InputC,
    ShootC,
    UIC,
    MovementC,
    PhysicsC,
    HealthC,
    AnimatorC,
    SoundC,
    RenderC,

    //^^^^^^ECS  SYSTEMS^^^^^^//
    
    c_type_last
};
typedef enum c_type ComponentType;

//ALL EXISTING COMPONENTS:
struct EntityComponentSystem
{
    aiv_vector *entitiesToDestroy;   //add an existing entity here when it is marked to be destroyed, remove it when it is destroyed
    aiv_vector *componentsToDestroy; //add an existing component here when it is marked to be destroyed, remove it when it is destroyed
    aiv_vector *entities;            //add an entity here when it is created, remove it when it is destroyed
    aiv_vector *systems;             //add a component to the right index in here when it is added to an entity, remove it when it or the owning entity are destroyed
};

struct Game;
struct Component
{
    int __systemIndex;                                                          //the index of this component in its system list, useful to find it or remove it immediately
    void *data;                                                                 //the real component defining its own behaviour
    struct Entity *owner;                                                       //the entity who owns this component
    ComponentType type;                                                         //useful to help finding the component in the list of components attached to an entity
    boolean active;                                                             //if this is true and the entity is also active, behaviour WILL be called, else not
    boolean markedToDestroy;                                                    //if this is true, the component will be destroyed at the end of the current frame
    void (*behaviour)(struct Component *selfComponent, struct Game* game);      //when the system updates this component, this behaviour is called, but only if active is true
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