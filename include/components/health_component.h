#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H
#include "ecs.h"

//HEALTH SYSTEM COMPONENT
struct HealthComponent
{
    int maxLives;   //the max lives this entity can have
    int currentLives;   //the current lives this entity has
    float maxHealth;    //the max health this entity can have
    float currentHealth;    //the current health this entity has
    void (*ChangeHealth)(struct HealthComponent* selfComponent, float amount); //the function called whenever you want to change the current health amount on this component
    void (*Die)(struct Component* selfComponent, struct Game* game); //the function called whenever the health is too low an you want to kill the entity
};

//INIT
void InitHealthComponent(struct HealthComponent *healthComponent, int maxLives, int startingLives, float maxHealth, float startingHealth, void (*ChangeHealth)(struct HealthComponent *selfComponent, float amount), void (*Die)(struct Component *selfComponent, struct Game *game));

//UPDATE
void HealthBehaviour(struct Component *selfComponent, struct Game* game);

//CUSTOM
void ChangeHealth(struct HealthComponent *selfComponent, float amount);
void PlayerDeath(struct Component* selfComponent, struct Game* game);
void EnemyDeath(struct Component* selfComponent, struct Game* game);
void BossDeath(struct Component* selfComponent, struct Game* game);

#endif //HEALTH_COMPONENT_H