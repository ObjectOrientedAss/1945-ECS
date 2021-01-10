#include "shoot_component.h"

void EnemyShootBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct ShootComponent *shootComponent = selfComponent->data;
    shootComponent->shootCooldownElapsed += game->engine->time->deltaTimeInSeconds;
    //printf("Shoot CD Elapsed: %d", shootComponent->shootCooldownElapsed);
    if (shootComponent->shootCooldownElapsed >= shootComponent->shootCooldown)
    {
        //printf("SHOOT!");
        shootComponent->shootCooldownElapsed = 0;
        struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);
        struct Entity *bullet = Dequeue(game->engine->poolsEngine, shootComponent->bulletType);
        InitTransformComponent(GetComponentData(bullet, TransformC), vec2_new(tc->position.x, tc->position.y + 15));
        SetEntityActiveStatus(bullet, true);
        Enqueue(game->engine->poolsEngine, bullet);

        struct Entity *audioEmitter = Dequeue(game->engine->poolsEngine, AudioEmitter);
        struct Component *c = GetComponent(audioEmitter, AudioC);
        struct AudioComponent *audioComponent = c->data;

        struct TimedBehaviourComponent *tbc = GetComponentData(audioEmitter, TimedBehaviourC);
        tbc->time = 0.2f;
        audioComponent->SetAudio(c, Explosion1SFX, 0);
        SetEntityActiveStatus(audioEmitter, true);
        audioComponent->PlayAudio(c, game);
        Enqueue(game->engine->poolsEngine, audioEmitter);
    }
}

void PlayerShootBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct ShootComponent *shootComponent = selfComponent->data;
    shootComponent->shootCooldownElapsed += game->engine->time->deltaTimeInSeconds;
    //printf("Shoot CD Elapsed: %d", shootComponent->shootCooldownElapsed);
    if (shootComponent->shootCooldownElapsed >= shootComponent->shootCooldown)
    {
        struct InputComponent *inputComponent = GetComponentData(selfComponent->owner, InputC);

        if (inputComponent->shoot)
        {
            shootComponent->shootCooldownElapsed = 0;
            struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);
            struct Entity *bullet = Dequeue(game->engine->poolsEngine, shootComponent->bulletType);
            InitTransformComponent(GetComponentData(bullet, TransformC), vec2_new(tc->position.x, tc->position.y - 20));
            SetEntityActiveStatus(bullet, true);
            Enqueue(game->engine->poolsEngine, bullet);

            struct Entity *audioEmitter = Dequeue(game->engine->poolsEngine, AudioEmitter);
            struct Component *c = GetComponent(audioEmitter, AudioC);
            struct AudioComponent *audioComponent = c->data;

            struct TimedBehaviourComponent *tbc = GetComponentData(audioEmitter, TimedBehaviourC);
            tbc->time = 0.2f;
            audioComponent->SetAudio(c, Explosion1SFX, 0);
            SetEntityActiveStatus(audioEmitter, true);
            audioComponent->PlayAudio(c, game);
            Enqueue(game->engine->poolsEngine, audioEmitter);
        }
    }
}

void InitShootComponent(struct ShootComponent *selfComponent, float shootCooldown, EntityType bulletType)
{
    selfComponent->shootCooldown = shootCooldown;
    selfComponent->shootCooldownElapsed = 0;
    selfComponent->bulletType = bulletType;
}