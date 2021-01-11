#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H
#include "ecs.h"

//HEALTH COMPONENT BEHAVIOUR: should manage health changes and check if the current health of the entity is <= 0 to trigger death if it happens
struct HealthComponent
{
    int maxLives;
    int currentLives;
    float currentHealth;
    float maxHealth;
    void (*ChangeHealth)(struct HealthComponent* selfComponent, float amount);
    void (*Die)(struct Component* selfComponent, struct Game* game);
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