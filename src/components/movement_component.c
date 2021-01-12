#include "movement_component.h"

void BossEntranceMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct MovementComponent *movementComponent = selfComponent->data;
    struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);

    tc->position.x += movementComponent->velocity.x * movementComponent->speed * game->engine->time->deltaTime;
    tc->position.y += movementComponent->velocity.y * movementComponent->speed * game->engine->time->deltaTime;

    if (tc->position.y >= 50)
    {
        tc->position.y = 50;
        movementComponent->velocity.y = 0;
        movementComponent->velocity.x = 1;
        selfComponent->behaviour = BossFightMovementBehaviour;
    }
}

void BossFightMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct MovementComponent *movementComponent = selfComponent->data;
    struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);

    tc->position.x += movementComponent->velocity.x * movementComponent->speed * game->engine->time->deltaTime;
    tc->position.y += movementComponent->velocity.y * movementComponent->speed * game->engine->time->deltaTime;

    if (tc->position.x > game->engine->GfxEngine->windowWidth - 50)
    {
        movementComponent->velocity.x = -movementComponent->velocity.x;
        tc->position.x = game->engine->GfxEngine->windowWidth - 50;
    }
    else if (tc->position.x < 50)
    {
        movementComponent->velocity.x = -movementComponent->velocity.x;
        tc->position.x = 50;
    }
}

void ParticleMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct MovementComponent *movementComponent = selfComponent->data;
    struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);

    tc->position.x += movementComponent->velocity.x * movementComponent->speed * game->engine->time->deltaTime;
    tc->position.y += movementComponent->velocity.y * movementComponent->speed * game->engine->time->deltaTime;
}

void AutomatedMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct MovementComponent *movementComponent = selfComponent->data;
    struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);

    tc->position.x += movementComponent->velocity.x * movementComponent->speed * game->engine->time->deltaTime;
    tc->position.y += movementComponent->velocity.y * movementComponent->speed * game->engine->time->deltaTime;

    if ((tc->position.y > game->engine->GfxEngine->windowHeight + 100 && movementComponent->velocity.y > 0) ||
        (tc->position.y < -100 && movementComponent->velocity.y < 0))
    {
        SetEntityActiveStatus(selfComponent->owner, false);
    }
}

void WaterMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct MovementComponent *movementComponent = selfComponent->data;
    struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);

    tc->position.x += movementComponent->velocity.x * movementComponent->speed * game->engine->time->deltaTime;
    tc->position.y += movementComponent->velocity.y * movementComponent->speed * game->engine->time->deltaTime;

    if (tc->position.y >= game->engine->GfxEngine->windowHeight + (game->engine->GfxEngine->windowHeight * 0.5f))
        tc->position.y = -game->engine->GfxEngine->windowHeight * 0.5f;
}

void PlayerMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct MovementComponent *movementComponent = selfComponent->data;
    struct InputComponent *ic = GetComponentData(selfComponent->owner, InputC);
    struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);

    movementComponent->velocity.x = ic->xAxis;
    movementComponent->velocity.y = ic->yAxis;

    tc->position.x += movementComponent->velocity.x * movementComponent->speed * game->engine->time->deltaTime;
    tc->position.y += movementComponent->velocity.y * movementComponent->speed * game->engine->time->deltaTime;

    if (tc->position.x < 0)
        tc->position.x = 0;
    else if (tc->position.x > game->engine->GfxEngine->windowWidth)
        tc->position.x = game->engine->GfxEngine->windowWidth;

    if (tc->position.y < 0)
        tc->position.y = 0;
    else if (tc->position.y > game->engine->GfxEngine->windowHeight - 76)
        tc->position.y = game->engine->GfxEngine->windowHeight - 76;
}

void InitMovementComponent(struct MovementComponent *movementComponent, vec2 velocity, float speed)
{
    movementComponent->velocity = velocity;
    movementComponent->speed = speed;
}