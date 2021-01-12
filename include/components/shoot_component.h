#ifndef SHOOT_COMPONENT_H
#define SHOOT_COMPONENT_H
#include "ecs.h"

//SHOOT SYSTEM COMPONENT
struct ShootComponent
{
    double shootCooldown;   //the time that must pass between a shoot and the next one
    double shootCooldownElapsed;    //the time passed since the last shoot
    EntityType bulletType;  //the bullet family to spawn when the shoot is triggered
};

//INIT
void InitShootComponent(struct ShootComponent* selfComponent, float shootCooldown, EntityFamily bulletType);

//UPDATE
void EnemyShootBehaviour(struct Component* selfComponent, struct Game* game);
void PlayerShootBehaviour(struct Component* selfComponent, struct Game* game);
void BossDoubleShootBehaviour(struct Component *selfComponent, struct Game *game);
void BossTripleShootBehaviour(struct Component *selfComponent, struct Game *game);

#endif //SHOOT_COMPONENT_H