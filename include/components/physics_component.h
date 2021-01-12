#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H
#include "ecs.h"
#include "timed_behaviour_component.h"

//the layermasks of each entity family with a collision system
enum c_layer
{
    PLAYER_COLLISION_LAYER = 0x01,
    ENEMY_COLLISION_LAYER = 0x02,
    PLAYERBULLET_COLLISION_LAYER = 0x04,
    ENEMYBULLET_COLLISION_LAYER = 0x08,
    POWERUP_COLLISION_LAYER = 0x10
};
typedef enum c_layer CollisionLayer;

//PHYSICS SYSTEM COMPONENT
struct PhysicsComponent
{
    CollisionLayer layersBitmask; //with who can this component collide
    CollisionLayer selfLayer; //which bit do this component occupy in the collision layers list
    float colliderRadius;  //the collision radius of this component
    float collisionsBlockTime; //the time that must pass before this entity will be able to collide again
    float collisionsBlockTimeElapsed; //the time passed since the last collision on this entity
    void (*Collide)(struct Component *selfComponent, struct Component *otherComponent, struct Game* game); //the function called when the component detects a collision with another physics component
    boolean canCollide;    //has this collider collided in this frame?
};

//INIT
void InitPhysicsComponent(struct Component *selfComponent, void (*Collide)(struct Component* selfComponent, struct Component* otherComponent, struct Game* game), boolean isTrigger, float colliderRadius);

//UPDATE
void PhysicsBehaviour(struct Component *selfComponent, struct Game* game);

//CUSTOM
void Collide(struct Component *selfComponent, struct Component *otherComponent, struct Game* game);

#endif //PHYSICS_COMPONENT_H