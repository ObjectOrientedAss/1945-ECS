#ifndef COMPONENT_H
#define COMPONENT_H

#include "entity.h"
#include "common.h"

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

//Basic struct containing the component data.
struct Component
{
    int __systemIndex;                                                          //the index of this component in its system list, useful to find it or remove it immediately. It is automatically handled by the ECS, do not interact with it.
    struct Entity *owner;                                                       //the entity who owns this component
    ComponentType type;                                                         //useful to help finding the component in the list of components attached to an entity
    void (*behaviour)(struct Component *selfComponent, struct Game* game);      //when the system updates this component, this behaviour is called, but only if active is true
    void *data;                                                                 //the real component defining its own behaviour
    boolean active;                                                             //if this is true and the entity is also active, behaviour WILL be called, else not
    boolean __markedToDestroy;                                                  //if this is true, the component will be destroyed at the end of the current frame
};

#endif //COMPONENT_H