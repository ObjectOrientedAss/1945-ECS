#ifndef SHOOT_COMPONENT_H
#define SHOOT_COMPONENT_H
#include "ecs.h"

//SHOOT COMPONENT BEHAVIOUR: should spawn a bullet if the condition to shoot is valid
struct ShootComponent
{
    double shootCooldown;
    double shootCooldownElapsed;
    EntityType bulletType;
};

//INIT
void InitShootComponent(struct ShootComponent* selfComponent, float shootCooldown, EntityType bulletType);

//UPDATE
void EnemyShootBehaviour(struct Component* selfComponent, struct Game* game);
void PlayerShootBehaviour(struct Component* selfComponent, struct Game* game);

#endif //SHOOT_COMPONENT_H