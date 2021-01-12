#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H
#include "ecs.h"
#include "vec2.h"

//MOVEMENT SYSTEM COMPONENT
struct MovementComponent
{
    vec2 velocity;  //the current velocity of this entity
    float speed;    //the movement speed of this entity
};

//INIT
void InitMovementComponent(struct MovementComponent* movementComponent, vec2 velocity, float speed);

//UPDATE
void BossEntranceMovementBehaviour(struct Component *selfComponent, struct Game *game);
void BossFightMovementBehaviour(struct Component *selfComponent, struct Game *game);
void ParticleMovementBehaviour(struct Component *selfComponent, struct Game *game);
void WaterMovementBehaviour(struct Component* selfComponent, struct Game* game);
void PlayerMovementBehaviour(struct Component *selfComponent, struct Game* game);
void AutomatedMovementBehaviour(struct Component *selfComponent, struct Game* game);


#endif //MOVEMENT_COMPONENT_H