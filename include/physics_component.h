#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H
#include "ecs.h"
#include "timed_behaviour_component.h"

enum c_layer
{
    PLAYER_COLLISION_LAYER = 0x01,
    ENEMY_COLLISION_LAYER = 0x02,
    PLAYERBULLET_COLLISION_LAYER = 0x04,
    ENEMYBULLET_COLLISION_LAYER = 0x08,
    POWERUP_COLLISION_LAYER = 0x10
};
typedef enum c_layer CollisionLayer;

//PHYSICS COMPONENT BEHAVIOUR: should check if the entity is colliding with another entity if it has the same component, and if that's so, trigger the collision on both
struct PhysicsComponent
{
    CollisionLayer layersBitmask; //with who can this component collide?
    CollisionLayer selfLayer; //which bit do this component occupy?
    float colliderRadius;  //the collision radius of this component
    float collisionsBlockTime; //the time that must pass before this component will be able to collide again
    float collisionsBlockTimeElapsed; //the time passed since the last collision
    void (*Collide)(struct Component *selfComponent, struct Component *otherComponent, struct Game* game);
    boolean canCollide;    //has this collider collided in this frame?
};

//INIT
void InitPhysicsComponent(struct Component *selfComponent, void (*Collide)(struct Component* selfComponent, struct Component* otherComponent, struct Game* game));

//UPDATE
void PhysicsBehaviour(struct Component *selfComponent, struct Game* game);

//CUSTOM
void Collide(struct Component *selfComponent, struct Component *otherComponent, struct Game* game);

#endif //PHYSICS_COMPONENT_H