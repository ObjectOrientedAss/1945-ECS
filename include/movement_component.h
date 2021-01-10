#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H
#include "ecs.h"
#include "vec2.h"

//MOVEMENT COMPONENT BEHAVIOUR: should move the entity
struct MovementComponent
{
    vec2 velocity;
    float speed;
};

//INIT
void InitMovementComponent(struct MovementComponent* movementComponent, vec2 velocity, float speed);

//UPDATE
void ParticleMovementBehaviour(struct Component *selfComponent, struct Game *game);
void WaterMovementBehaviour(struct Component* selfComponent, struct Game* game);
void PlayerMovementBehaviour(struct Component *selfComponent, struct Game* game);
void AutomatedMovementBehaviour(struct Component *selfComponent, struct Game* game);


#endif //MOVEMENT_COMPONENT_H