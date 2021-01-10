#include "health_component.h"

void HealthBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Health Behaviour Called\n");
    //get my health component
    struct HealthComponent *healthComponent = selfComponent->data;
    if (healthComponent->currentHealth <= 0)
    {
        printf("\nHealth <= 0, DIE!");
        healthComponent->Die(selfComponent, game);
    }
}

void PlayerDeath(struct Component *selfComponent, struct Game *game)
{
    //the component arriving here is a healthcomponent
    selfComponent->active = false;
    struct HealthComponent *healthComponent = selfComponent->data;
    healthComponent->currentLives--;
    struct BattleLevelData *bld = game->levelData;
    struct Entity *lifeUI = aiv_vector_at(bld->lives, healthComponent->currentLives);
    SetEntityActiveStatus(lifeUI, false);
    printf("\nLife lost. Current Lives: %d", healthComponent->currentLives);
    if (healthComponent->currentLives <= 0) //final life spent, goodbye my friend.
    {
        struct Component *rc = GetComponent(selfComponent->owner, RenderC);
        SetComponentActiveStatus(rc, false);

        struct Component *ic = GetComponent(selfComponent->owner, InputC);
        SetComponentActiveStatus(ic, false);

        struct Component *pc = GetComponent(selfComponent->owner, PhysicsC);
        SetComponentActiveStatus(pc, false);

        struct Component *sc = GetComponent(selfComponent->owner, ShootC);
        SetComponentActiveStatus(sc, false);

        struct Component *tb = AddComponent(selfComponent->owner, TimedBehaviourC, GoToMainMenuAfter);
        InitTimedBehaviourComponent(tb->data, 1, 3.0f, NULL);

        struct TransformComponent *transformComponent = GetComponentData(selfComponent->owner, TransformC);
        struct MovementComponent *movementComponent = GetComponentData(selfComponent->owner, MovementC);

        struct Entity *particle = Dequeue(game->engine->poolsEngine, ExplosionParticle);
        struct TransformComponent *otherTransformComponent = GetComponentData(particle, TransformC);
        struct MovementComponent *otherMovementComponent = GetComponentData(particle, MovementC);

        otherTransformComponent->position = transformComponent->position;
        otherMovementComponent->velocity = movementComponent->velocity;
        otherMovementComponent->speed = movementComponent->speed;

        movementComponent->velocity = vec2_new(0, 0);
        SetEntityActiveStatus(particle, true);
        Enqueue(game->engine->poolsEngine, particle);

        return;
    }

    struct Component *tb = AddComponent(selfComponent->owner, TimedBehaviourC, PlayerTimedRespawnBehaviour);
    InitTimedBehaviourComponent(tb->data, 1, 5.0f, NULL);

    tb = AddComponent(selfComponent->owner, TimedBehaviourC, AlphaBlendingBehaviour);
    aiv_vector *args = aiv_vector_new_with_cap(4);
    int *currentAlpha = malloc(1 * sizeof(int));    //currentAlpha - INT
    int *direction = malloc(1 * sizeof(int));       //direction - INT
    float *accumulator = malloc(1 * sizeof(float)); //accumulator - FLOAT
    float *fadeSpeed = malloc(1 * sizeof(float));   //fadeSpeed - FLOAT
    *currentAlpha = 255;
    *direction = -1;
    *accumulator = 0.0f;
    *fadeSpeed = 5.0f;

    aiv_vector_add(args, currentAlpha);
    aiv_vector_add(args, direction);
    aiv_vector_add(args, accumulator);
    aiv_vector_add(args, fadeSpeed);
    InitTimedBehaviourComponent(tb->data, 1, 3.0f, args);
}

void EnemyDeath(struct Component *selfComponent, struct Game *game)
{
    //spawn particle
    struct Entity *particle = Dequeue(game->engine->poolsEngine, ExplosionParticle);
    struct TransformComponent *transformComponent = GetComponentData(selfComponent->owner, TransformC);
    struct MovementComponent *movementComponent = GetComponentData(selfComponent->owner, MovementC);

    struct TransformComponent *otherTransformComponent = GetComponentData(particle, TransformC);
    struct MovementComponent *otherMovementComponent = GetComponentData(particle, MovementC);

    otherTransformComponent->position = transformComponent->position;
    otherMovementComponent->velocity = movementComponent->velocity;
    otherMovementComponent->speed = movementComponent->speed;
    SetEntityActiveStatus(particle, true);
    Enqueue(game->engine->poolsEngine, particle);

    //spawn audio emitter
    struct Entity *audioEmitter = Dequeue(game->engine->poolsEngine, AudioEmitter);
    struct Component *c = GetComponent(audioEmitter, AudioC);
    struct AudioComponent *audioComponent = c->data;
    struct TimedBehaviourComponent *tbc = GetComponentData(audioEmitter, TimedBehaviourC);
    tbc->time = 1.2f;
    audioComponent->SetAudio(c, Explosion2SFX, 0);
    SetEntityActiveStatus(audioEmitter, true);
    Enqueue(game->engine->poolsEngine, audioEmitter);
    audioComponent->PlayAudio(c, game);

    //increase score
    struct BattleLevelData *bld = game->levelData;
    bld->score++;
    sprintf(bld->scoreToString, "%d", bld->score);
    bld->scoreUI->text = bld->scoreToString;

    //spawn life power up, if you're lucky
    float lifePowerUpExtraction = GetRandomFloat(100.0f);
    if (lifePowerUpExtraction <= bld->lifePowerupSpawnChance)
    {
        struct Entity *powerup = Dequeue(game->engine->poolsEngine, LifePowerup);
        InitTransformComponent(GetComponentData(powerup, TransformC), transformComponent->position);
        SetEntityActiveStatus(powerup, true);
        Enqueue(game->engine->poolsEngine, powerup);
        //movementComponent->velocity.x = 0; //reset x velocity in case this enemy was using an ondulation component while alive
    }

    SetEntityActiveStatus(selfComponent->owner, false);
}

void ChangeHealth(struct HealthComponent *selfComponent, float amount)
{
    selfComponent->currentHealth += amount;
    if (selfComponent->currentHealth > selfComponent->maxHealth)
        selfComponent->currentHealth = selfComponent->maxHealth;
}


void InitHealthComponent(struct HealthComponent *healthComponent, int maxLives, int startingLives, float maxHealth, float startingHealth, void (*ChangeHealth)(struct HealthComponent *selfComponent, float amount), void (*Die)(struct Component *selfComponent, struct Game *game))
{
    healthComponent->maxHealth = maxHealth;
    healthComponent->currentHealth = startingHealth;
    healthComponent->ChangeHealth = ChangeHealth;
    healthComponent->Die = Die;
    healthComponent->maxLives = maxLives;
    healthComponent->currentLives = startingLives;

    printf("\n---Health Component Initialized!");
}