#include "behaviours.h"
#include "stdio.h"
#include <math.h>

void InputBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Input Behaviour Called\n");
    InputComponent *inputComponent = (InputComponent *)selfComponent->data;
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    //inputComponent->shoot = false;

    //horizontal input
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
        inputComponent->xAxis = -1.f;
    else if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
        inputComponent->xAxis = 1.f;
    else
        inputComponent->xAxis = 0;

    //vertical input
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
        inputComponent->yAxis = -1.f;
    else if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
        inputComponent->yAxis = 1.f;
    else
        inputComponent->yAxis = 0;

    //shoot input
    if (state[SDL_SCANCODE_SPACE])
        inputComponent->shoot = true;
    else
        inputComponent->shoot = false;
}

void UIBehaviour(struct Component *selfComponent, struct Game *game)
{
    UIComponent *uiComponent = (UIComponent *)selfComponent->data;

    RenderComponent *renderComponent = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);
    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);

    //if the mouse is over the button
    if (game->mousePositionX >= tc->position.x - (renderComponent->sprite.spriteRect.w * 0.5f) &&
        game->mousePositionX <= tc->position.x + (renderComponent->sprite.spriteRect.w * 0.5f) &&
        game->mousePositionY >= tc->position.y - (renderComponent->sprite.spriteRect.h * 0.5f) &&
        game->mousePositionY <= tc->position.y + (renderComponent->sprite.spriteRect.h * 0.5f))
    {
        //printf("Mouse is over the button!!!!!!!!!");
        //if it is the first frame in which the mouse has started hovering
        if (!uiComponent->isHovering)
        {
            uiComponent->isHovering = true;
            if (uiComponent->OnHoverStart != NULL)
                uiComponent->OnHoverStart(selfComponent);
        }
        else
        {
            if (game->mouseDown)
                uiComponent->OnClick(selfComponent, game);
        }
    }
    else //if the mouse is not over the button
    {
        if (uiComponent->isHovering)
        {
            uiComponent->isHovering = false;
            if (uiComponent->OnHoverEnd != NULL)
                uiComponent->OnHoverEnd(selfComponent);
        }
    }
}

void WaterMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    MovementComponent *movementComponent = (MovementComponent *)selfComponent->data;
    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);

    tc->position.x += movementComponent->velocity.x * movementComponent->speed * game->engine->time->deltaTime;
    tc->position.y += movementComponent->velocity.y * movementComponent->speed * game->engine->time->deltaTime;

    if (tc->position.y >= game->engine->GfxEngine->windowHeight + (game->engine->GfxEngine->windowHeight * 0.5f))
        tc->position.y = -game->engine->GfxEngine->windowHeight * 0.5f;
}

void PlayerMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("PlayerMovement Behaviour Called\n");
    MovementComponent *movementComponent = (MovementComponent *)selfComponent->data;
    InputComponent *ic = (InputComponent *)GetComponentData(selfComponent->owner, InputC);
    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);

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
    else if (tc->position.y > game->engine->GfxEngine->windowHeight)
        tc->position.y = game->engine->GfxEngine->windowHeight;
}

void AutomatedMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Enemy Behaviour Called\n");
    MovementComponent *movementComponent = (MovementComponent *)selfComponent->data;
    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);

    tc->position.x += movementComponent->velocity.x * movementComponent->speed * game->engine->time->deltaTime;
    tc->position.y += movementComponent->velocity.y * movementComponent->speed * game->engine->time->deltaTime;

    if ((tc->position.y > game->engine->GfxEngine->windowHeight + 100 && movementComponent->velocity.y > 0) ||
        (tc->position.y < -100 && movementComponent->velocity.y < 0))
    {
        SetEntityActiveStatus(selfComponent->owner, false);
        //printf("DISATTIVATA ENTITY NUMERO %d ALLE COORDINATE:    %f  |  %f", selfComponent->owner->__entityIndex, tc->position.x, tc->position.y);
    }
}

void PhysicsBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Physics Behaviour Called\n");
    //get my physics component
    PhysicsComponent *physicsComponent = (PhysicsComponent *)selfComponent->data;

    if (!physicsComponent->canCollide)
    {
        physicsComponent->collisionsBlockTimeElapsed += game->engine->time->deltaTimeInSeconds; //the time passed since the last collision
        if (physicsComponent->collisionsBlockTimeElapsed >= physicsComponent->collisionsBlockTime)
        {
            physicsComponent->collisionsBlockTimeElapsed = 0;
            physicsComponent->canCollide = true;
        }
        else
            return;
    }

    //get my movement component
    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);

    //iterate all the other physics components!
    aiv_vector *physicsSystem = (aiv_vector *)aiv_vector_at(selfComponent->owner->ECS->systems, (int)selfComponent->type);
    for (uint i = 0; i < aiv_vector_size(physicsSystem); i++)
    {
        struct Component *otherComponent = (struct Component *)aiv_vector_at(physicsSystem, i);
        if (!otherComponent->owner->active)
            continue;

        //get the other physics component
        PhysicsComponent *otherPhysicsComponent = (PhysicsComponent *)otherComponent->data;
        if (!otherPhysicsComponent->canCollide) //the other collider has already collided in this frame!
            continue;
        if (physicsComponent == otherPhysicsComponent) //i am also in the same list, so, if this is myself...
            continue;

        //check if the collision layers are compatible
        if ((physicsComponent->selfLayer & otherPhysicsComponent->layersBitmask))
        {
            //printf("\n----Type %d can collide with Type %d", selfComponent->owner->type, otherComponent->owner->type);
            //get the other transform component and relative position
            TransformComponent *otherTransformComponent = (TransformComponent *)GetComponentData(otherComponent->owner, TransformC);
            //get the distance vector between me and the other
            vec2 vDist = vec2_new(0, 0);
            vDist = vec2_sub(&tc->position, &otherTransformComponent->position);
            //if the distance is less than the two colliders radiuses combined, we are colliding on each other
            if (vec2_magn(&vDist) <= physicsComponent->colliderRadius + otherPhysicsComponent->colliderRadius)
            {
                //printf("\n\n---------     COLLISION HAPPENED BETWEEN %d AND %d!", selfComponent->owner->type, otherComponent->owner->type);
                //handle the collision on me:
                physicsComponent->canCollide = false;
                physicsComponent->Collide(selfComponent, otherComponent);
                //trigger and handle the collision on the other:
                otherPhysicsComponent->canCollide = false;
                otherPhysicsComponent->Collide(otherComponent, selfComponent);
                break; //i just want to be able to collide once per frame, so after colliding, stop checking collisions on this object!
            }
        }
    }
}

void HealthBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Health Behaviour Called\n");
    //get my health component
    HealthComponent *healthComponent = (HealthComponent *)selfComponent->data;
    if (healthComponent->currentHealth <= 0)
    {
        printf("\nHealth <= 0, DIE!");
        healthComponent->Die(selfComponent, game);
    }
}

void AnimatorBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Animator Behaviour Called\n");
    AnimatorComponent *animatorComponent = (AnimatorComponent *)selfComponent->data;

    if (animatorComponent->currentAnimation.frames <= 1) //if an asshole has created an animation spritesheet with 1 or less frames...
        return;

    RenderComponent *renderComponent = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);

    animatorComponent->timeElapsed += game->engine->time->deltaTimeInSeconds;
    if (animatorComponent->timeElapsed >= animatorComponent->frameDuration)
    {
        animatorComponent->timeElapsed = 0;
        animatorComponent->currentFrameIndex++;
        if (animatorComponent->currentFrameIndex >= animatorComponent->currentAnimation.frames)
            animatorComponent->currentFrameIndex = 0;

        renderComponent->sprite.originRect.x = animatorComponent->currentFrameIndex * animatorComponent->currentAnimation.frameWidth;
    }
}

void RenderBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Render Behaviour Called\n");
    RenderComponent *renderComponent = (RenderComponent *)selfComponent->data;

    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);
    renderComponent->sprite.spriteRect.x = (int)tc->position.x - (int)(renderComponent->sprite.spriteRect.w * 0.5f);
    renderComponent->sprite.spriteRect.y = (int)tc->position.y - (int)(renderComponent->sprite.spriteRect.h * 0.5f);

    SDL_RenderCopy(renderComponent->renderer, renderComponent->sprite.texture, &renderComponent->sprite.originRect, &renderComponent->sprite.spriteRect);
}

void FontRenderBehaviour(struct Component *selfComponent, struct Game *game)
{
    RenderComponent *renderComponent = (RenderComponent *)selfComponent->data;

    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);
    renderComponent->sprite.spriteRect.x = (int)tc->position.x - (int)(renderComponent->sprite.spriteRect.w * 0.5f);
    renderComponent->sprite.spriteRect.y = (int)tc->position.y - (int)(renderComponent->sprite.spriteRect.h * 0.5f);

    TextComponent *textComponent = (TextComponent *)GetComponentData(selfComponent->owner, TextC);
    FC_Draw(GetFont(game->engine->GfxEngine, textComponent->fontType), renderComponent->renderer, renderComponent->sprite.spriteRect.x, renderComponent->sprite.spriteRect.y, textComponent->text);
}

void EnemyShootBehaviour(struct Component *selfComponent, struct Game *game)
{
    ShootComponent *shootComponent = (ShootComponent *)selfComponent->data;
    shootComponent->shootCooldownElapsed += game->engine->time->deltaTimeInSeconds;
    //printf("Shoot CD Elapsed: %d", shootComponent->shootCooldownElapsed);
    if (shootComponent->shootCooldownElapsed >= shootComponent->shootCooldown)
    {
        //printf("SHOOT!");
        shootComponent->shootCooldownElapsed = 0;
        TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);
        struct Entity *bullet = Dequeue(game->engine->poolsEngine, shootComponent->bulletType);
        InitTransformComponent(GetComponentData(bullet, TransformC), vec2_new(tc->position.x, tc->position.y + 15));
        SetEntityActiveStatus(bullet, true);
        Enqueue(game->engine->poolsEngine, bullet);

        struct Entity *audioEmitter = Dequeue(game->engine->poolsEngine, AudioEmitter);
        struct Component *c = (struct Component *)GetComponent(audioEmitter, AudioC);
        AudioComponent *audioComponent = (AudioComponent *)c->data;

        TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)GetComponentData(audioEmitter, TimedBehaviourC);
        tbc->time = 0.2f;
        audioComponent->SetAudio(c, Explosion1SFX, 0);
        SetEntityActiveStatus(audioEmitter, true);
        audioComponent->PlayAudio(c, game);
        Enqueue(game->engine->poolsEngine, audioEmitter);
    }
}

void PlayerShootBehaviour(struct Component *selfComponent, struct Game *game)
{
    ShootComponent *shootComponent = (ShootComponent *)selfComponent->data;
    shootComponent->shootCooldownElapsed += game->engine->time->deltaTimeInSeconds;
    //printf("Shoot CD Elapsed: %d", shootComponent->shootCooldownElapsed);
    if (shootComponent->shootCooldownElapsed >= shootComponent->shootCooldown)
    {
        InputComponent *inputComponent = (InputComponent *)GetComponentData(selfComponent->owner, InputC);

        if (inputComponent->shoot)
        {
            shootComponent->shootCooldownElapsed = 0;
            TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);
            struct Entity *bullet = Dequeue(game->engine->poolsEngine, shootComponent->bulletType);
            InitTransformComponent(GetComponentData(bullet, TransformC), vec2_new(tc->position.x, tc->position.y - 20));
            SetEntityActiveStatus(bullet, true);
            Enqueue(game->engine->poolsEngine, bullet);

            struct Entity *audioEmitter = Dequeue(game->engine->poolsEngine, AudioEmitter);
            struct Component *c = (struct Component *)GetComponent(audioEmitter, AudioC);
            AudioComponent *audioComponent = (AudioComponent *)c->data;

            TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)GetComponentData(audioEmitter, TimedBehaviourC);
            tbc->time = 0.2f;
            audioComponent->SetAudio(c, Explosion1SFX, 0);
            SetEntityActiveStatus(audioEmitter, true);
            audioComponent->PlayAudio(c, game);
            Enqueue(game->engine->poolsEngine, audioEmitter);
        }
    }
}

void SpawnIslandBehaviour(struct Component *selfComponent, struct Game *game)
{
    TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);
        struct Entity *island = Dequeue(game->engine->poolsEngine, Background);
        InitTransformComponent(GetComponentData(island, TransformC), vec2_new(GetRandomFloatBetween(30.0f, 620.0f), tc->position.y));
        SetEntityActiveStatus(island, true);

        //pick a random island sprite
        int randomIslandSprite = GetRandomIntBetween(Island3S, Island1S);
        InitRenderComponent(GetComponentData(island, RenderC), game->engine->GfxEngine, randomIslandSprite);
        Enqueue(game->engine->poolsEngine, island);

        struct BattleLevelData *bld = game->levelData;
        tbc->time = bld->islandSpawnTimer + GetRandomFloatBetween(bld->islandSpawnTimerChanger, -bld->islandSpawnTimerChanger);
    }
}

void SpawnEnemyBehaviour(struct Component *selfComponent, struct Game *game)
{
    TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);
        struct Entity *enemy = Dequeue(game->engine->poolsEngine, Enemy);
        InitTransformComponent(GetComponentData(enemy, TransformC), vec2_new(GetRandomFloatBetween(15.0f, 625.0f), tc->position.y));
        int randomEnemyAnimation = GetRandomIntBetween(Enemy4A, Enemy1A);
        struct Component *c = GetComponent(enemy, AnimatorC);
        AnimatorComponent *ac = c->data;
        ac->SetAnimation(c, randomEnemyAnimation, 0);
        HealthComponent *hc = GetComponentData(enemy, HealthC);
        hc->currentHealth = hc->maxHealth;
        SetEntityActiveStatus(enemy, true);
        Enqueue(game->engine->poolsEngine, enemy);

        struct BattleLevelData *bld = game->levelData;
        tbc->time = bld->islandSpawnTimer + GetRandomFloatBetween(bld->enemySpawnTimerChanger, -bld->enemySpawnTimerChanger);

        int velocityChangeExtraction = GetRandomInt(100);
        if (velocityChangeExtraction <= bld->enemyOndulationChance)
        {
            struct Component *tb = AddComponent(enemy, TimedBehaviourC, OndulationBehaviour);
            InitTimedBehaviourComponent(tb->data, 10, 0.3f, NULL);
            //printf("E' successo...");
        }
        //printf("NON e' successo...");
    }
}

void ToggleEntityAfterBehaviour(struct Component *selfComponent, struct Game *game)
{
    TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        SetEntityActiveStatus(selfComponent->owner, !selfComponent->owner->active);
    }
}

void SpawnEnemySquadronBehaviour(struct Component *selfComponent, struct Game *game)
{
    TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);

        struct BattleLevelData *bld = game->levelData;
        int enemies = GetRandomIntBetween(bld->maxEnemySquadronSize, bld->minEnemySquadronSize);
        float y = tc->position.y;
        float x = GetRandomFloatBetween(15.0f, 625.0f);
        int xDirection = 1;
        for (int i = 0; i < enemies; i++)
        {
            struct Entity *enemy = Dequeue(game->engine->poolsEngine, Enemy);
            InitTransformComponent(GetComponentData(enemy, TransformC), vec2_new(x, y));
            int randomEnemyAnimation = GetRandomIntBetween(Enemy4A, Enemy1A);
            struct Component *c = GetComponent(enemy, AnimatorC);
            AnimatorComponent *ac = c->data;
            ac->SetAnimation(c, randomEnemyAnimation, 0);
            HealthComponent *hc = GetComponentData(enemy, HealthC);
            hc->currentHealth = hc->maxHealth;
            SetEntityActiveStatus(enemy, true);
            Enqueue(game->engine->poolsEngine, enemy);
            y -= 45;
            if (x + 45 >= 625 || x - 45 <= 15)
                xDirection = -xDirection;
            x += 45 * xDirection;
        }
    }
}

void ScoreDistanceBehaviour(struct Component *selfComponent, struct Game *game)
{
    TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct BattleLevelData *bld = game->levelData;
        bld->score += bld->increaseScore;
        sprintf(bld->scoreToString, "%d", bld->score);
        bld->scoreUI->text = bld->scoreToString;
    }
}

void OndulationBehaviour(struct Component *selfComponent, struct Game *game)
{
    TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)selfComponent->data;

    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        MovementComponent *mc = (MovementComponent *)GetComponentData(selfComponent->owner, MovementC);

        if (mc->velocity.x == 0)
            mc->velocity.x = 1;

        mc->velocity.x = -mc->velocity.x;
        tbc->currentRepetitions++;
        if (tbc->currentRepetitions >= tbc->repetitions)
        {
            tbc->currentRepetitions = 0;
            mc->velocity.x = 0;
            MarkComponentToDestroy(selfComponent);
        }
    }
}

void PlayerTimedRespawnBehaviour(struct Component *selfComponent, struct Game *game)
{
    TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct Component *c = (struct Component *)GetComponent(selfComponent->owner, HealthC);
        HealthComponent *hc = (HealthComponent *)c->data;
        hc->currentHealth = hc->maxHealth;
        c->active = true;
        MarkComponentToDestroy(selfComponent);
    }
}

void GoToMainMenuAfter(struct Component *selfComponent, struct Game *game)
{
    TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        game->sceneToLoad = MainMenuScene;
        struct BattleLevelData *bld = game->levelData;
        aiv_vector_destroy(bld->lives);

        //save high score, if it is beaten
        FILE* fp = fopen("resources/assets/scores/high_score.txt", "r");
        char scoreBuffer[5];
        fgets(scoreBuffer, 5, fp);
        printf("\nCurent HS: %s", scoreBuffer);

        int currentHighScore = atoi(scoreBuffer);
        fclose(fp);
        if(bld->score > currentHighScore)
        {
            fp = fopen("resources/assets/scores/high_score.txt", "w");
            fputs(bld->scoreToString, fp);
            printf("\nOverwriting high score with new score %s", scoreBuffer);
        }
        fclose(fp);

        MarkComponentToDestroy(selfComponent);
    }
}

void AlphaBlendingBehaviour(struct Component *selfComponent, struct Game *game)
{
    TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)selfComponent->data;

    int *currentAlpha = (int *)aiv_vector_at(tbc->customArgs, 0);    //currentAlpha - INT
    int *direction = (int *)aiv_vector_at(tbc->customArgs, 1);       //direction - INT
    float *accumulator = (float *)aiv_vector_at(tbc->customArgs, 2); //accumulator - FLOAT
    float *fadeSpeed = (float *)aiv_vector_at(tbc->customArgs, 3);   //fadeSpeed - FLOAT

    *accumulator += *fadeSpeed * game->engine->time->deltaTimeInSeconds * *direction;
    *currentAlpha = 255 * *accumulator;

    if (abs(*accumulator) > 1.0f)
    {
        *accumulator = 0.0f;
        *direction = -*direction;
    }

    RenderComponent *rc = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        SDL_SetTextureAlphaMod(rc->sprite.texture, 255);
        void *ptr = aiv_vector_remove_at(tbc->customArgs, aiv_vector_size(tbc->customArgs) - 1);
        free(ptr);
        aiv_vector_destroy(tbc->customArgs);
        MarkComponentToDestroy(selfComponent);
        return;
    }

    SDL_SetTextureAlphaMod(rc->sprite.texture, *currentAlpha);
}

//COMPONENTS CALLS FROM OUTSIDE-----------------------------------------------------------------------

void PlayerDeath(struct Component *selfComponent, struct Game *game)
{
    //the component arriving here is a healthcomponent
    selfComponent->active = false;
    HealthComponent *healthComponent = (HealthComponent *)selfComponent->data;
    healthComponent->currentLives--;
    struct BattleLevelData *bld = game->levelData;
    struct Entity *lifeUI = aiv_vector_at(bld->lives, healthComponent->currentLives);
    SetEntityActiveStatus(lifeUI, false);
    printf("\nLife lost. Current Lives: %d", healthComponent->currentLives);
    if (healthComponent->currentLives <= 0) //final life spent, goodbye my friend.
    {
        struct Component *rc = (struct Component *)GetComponent(selfComponent->owner, RenderC);
        SetComponentActiveStatus(rc, false);

        struct Component *ic = (struct Component *)GetComponent(selfComponent->owner, InputC);
        SetComponentActiveStatus(ic, false);

        struct Component *pc = (struct Component *)GetComponent(selfComponent->owner, PhysicsC);
        SetComponentActiveStatus(ic, false);

        struct Component *tb = AddComponent(selfComponent->owner, TimedBehaviourC, GoToMainMenuAfter);
        InitTimedBehaviourComponent(tb->data, 1, 3.0f, NULL);

        TransformComponent *transformComponent = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);
        MovementComponent *movementComponent = (MovementComponent *)GetComponentData(selfComponent->owner, MovementC);

        struct Entity *particle = Dequeue(game->engine->poolsEngine, Particle);
        TransformComponent *otherTransformComponent = (TransformComponent *)GetComponentData(particle, TransformC);
        MovementComponent *otherMovementComponent = (MovementComponent *)GetComponentData(particle, MovementC);

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
    struct Entity *particle = Dequeue(game->engine->poolsEngine, Particle);
    TransformComponent *transformComponent = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);
    MovementComponent *movementComponent = (MovementComponent *)GetComponentData(selfComponent->owner, MovementC);

    TransformComponent *otherTransformComponent = (TransformComponent *)GetComponentData(particle, TransformC);
    MovementComponent *otherMovementComponent = (MovementComponent *)GetComponentData(particle, MovementC);

    otherTransformComponent->position = transformComponent->position;
    otherMovementComponent->velocity = movementComponent->velocity;
    otherMovementComponent->speed = movementComponent->speed;
    SetEntityActiveStatus(particle, true);
    Enqueue(game->engine->poolsEngine, particle);

    //spawn audio emitter
    struct Entity *audioEmitter = Dequeue(game->engine->poolsEngine, AudioEmitter);
    struct Component *c = (struct Component *)GetComponent(audioEmitter, AudioC);
    AudioComponent *audioComponent = (AudioComponent *)c->data;
    TimedBehaviourComponent *tbc = (TimedBehaviourComponent *)GetComponentData(audioEmitter, TimedBehaviourC);
    tbc->time = 1.2f;
    audioComponent->SetAudio(c, Explosion2SFX, 0);
    SetEntityActiveStatus(audioEmitter, true);
    Enqueue(game->engine->poolsEngine, audioEmitter);
    audioComponent->PlayAudio(c, game);
    SetEntityActiveStatus(selfComponent->owner, false);

    struct BattleLevelData *bld = game->levelData;
    bld->score++;
    sprintf(bld->scoreToString, "%d", bld->score);
    bld->scoreUI->text = bld->scoreToString;
}

void Collide(struct Component *selfComponent, struct Component *otherComponent)
{
    //get my physics component
    PhysicsComponent *selfPhysicsComponent = (PhysicsComponent *)selfComponent->data;
    //get other physics component
    PhysicsComponent *otherPhysicsComponent = (PhysicsComponent *)otherComponent->data;

    switch (selfComponent->owner->type)
    {
    case Player:
        if (otherComponent->owner->type == Enemy)
        {
            HealthComponent *healthComponent = (HealthComponent *)GetComponentData(selfComponent->owner, HealthC);
            healthComponent->ChangeHealth(healthComponent, -50.0f);
            printf("\nPlayer health: %f", healthComponent->currentHealth);
        }
        else if (otherComponent->owner->type == EnemyBullet)
        {
            HealthComponent *healthComponent = (HealthComponent *)GetComponentData(selfComponent->owner, HealthC);
            healthComponent->ChangeHealth(healthComponent, -15.0f);
            printf("\nPlayer health: %f", healthComponent->currentHealth);
        }
        break;
    case Enemy:
        if (otherComponent->owner->type == Player)
        {
            HealthComponent *healthComponent = (HealthComponent *)GetComponentData(selfComponent->owner, HealthC);
            healthComponent->ChangeHealth(healthComponent, -healthComponent->maxHealth);
            printf("\nEnemy health: %f", healthComponent->currentHealth);
        }
        else if (otherComponent->owner->type == PlayerBullet)
        {
            HealthComponent *healthComponent = (HealthComponent *)GetComponentData(selfComponent->owner, HealthC);
            healthComponent->ChangeHealth(healthComponent, -35.0f);
            printf("\nEnemy health: %f", healthComponent->currentHealth);
        }
        break;
    case PlayerBullet:
        if (otherComponent->owner->type == Enemy)
            SetEntityActiveStatus(selfComponent->owner, false);
        break;
    case EnemyBullet:
        if (otherComponent->owner->type == Player)
            SetEntityActiveStatus(selfComponent->owner, false);
        break;
    }
}

void ChangeHealth(HealthComponent *selfComponent, float amount)
{
    selfComponent->currentHealth += amount;
    if (selfComponent->currentHealth > selfComponent->maxHealth)
        selfComponent->currentHealth = selfComponent->maxHealth;
}

//Set the animation to perform on the given animator component.
//If you also want to alter the frame duration of the spritesheet, pass a value > 0.
void SetAnimation(struct Component *selfComponent, AnimationType type, float frameDuration)
{
    AnimatorComponent *animatorComponent = (AnimatorComponent *)selfComponent->data;

    RenderComponent *renderComponent = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);

    animatorComponent->currentAnimation = GetAnimation(animatorComponent->engine, type);
    animatorComponent->frameDuration = animatorComponent->currentAnimation.baseFrameDuration;
    animatorComponent->timeElapsed = 0;
    animatorComponent->currentFrameIndex = 0;
    if (frameDuration > 0)
        animatorComponent->frameDuration = frameDuration;

    renderComponent->sprite.texture = animatorComponent->currentAnimation.texture;
    renderComponent->sprite.originRect.w = animatorComponent->currentAnimation.frameWidth;
    renderComponent->sprite.originRect.h = animatorComponent->currentAnimation.frameHeight;
    renderComponent->sprite.originRect.x = animatorComponent->currentFrameIndex * animatorComponent->currentAnimation.frameWidth;
    renderComponent->sprite.spriteRect.w = animatorComponent->currentAnimation.frameWidth;
    renderComponent->sprite.spriteRect.h = animatorComponent->currentAnimation.frameHeight;
}

//in this game i want all the buttons to have the same hover start and hover end behaviours so i can recycle them...

void ButtonHoverStart(struct Component *selfComponent)
{
    printf("Hovering on button started");
    RenderComponent *rc = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);
    rc->sprite.spriteRect.w += 10;
    rc->sprite.spriteRect.h += 10;
}

//
void ButtonHoverEnd(struct Component *selfComponent)
{
    printf("Hovering on button ended");
    RenderComponent *rc = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);
    rc->sprite.spriteRect.w -= 10;
    rc->sprite.spriteRect.h -= 10;
}

//the start button click event in the main menu
void StartGameClick(struct Component *selfComponent, struct Game *game)
{
    printf("Clicking on Start Game");
    game->sceneToLoad = GameScene;
}

//the quit button click event in the main menu
void QuitGameClick(struct Component *selfComponent, struct Game *game)
{
    printf("Clicking on Quit Game");
    game->quitRequest = true;
}

//set the audio on an audio component (it will play when the entity is active)
void SetAudio(struct Component *selfComponent, AudioType type, int loops)
{
    AudioComponent *audioComponent = (AudioComponent *)selfComponent->data;
    audioComponent->audio = GetSound(audioComponent->engine, type);
    audioComponent->loops = loops;
    audioComponent->isPlaying = false;
}

void PlayAudio(struct Component *selfComponent, struct Game *game)
{
    AudioComponent *audioComponent = selfComponent->data;
    switch (audioComponent->audio.audioExtension)
    {
    case WAV:
        Mix_PlayChannel(-1, audioComponent->audio.data, audioComponent->loops);
        break;
    case MP3:
        Mix_PlayMusic(audioComponent->audio.data, audioComponent->loops);
        break;
    }
}

//COMPONENTS INIT METHODS----------------------------------------------------------------------------------------

void InitTimedBehaviourComponent(TimedBehaviourComponent *timedBehaviourComponent, int repetitions, float time, aiv_vector *customArgs)
{
    timedBehaviourComponent->repetitions = repetitions;
    timedBehaviourComponent->currentRepetitions = 0;
    timedBehaviourComponent->time = time;
    timedBehaviourComponent->elapsedTime = 0;
    timedBehaviourComponent->customArgs = customArgs;
    printf("\n---Timed Behaviour Component Initialized!");
}

void InitMovementComponent(MovementComponent *movementComponent, vec2 velocity, float speed)
{
    movementComponent->velocity = velocity;
    movementComponent->speed = speed;
    printf("\n---Movement Component Initialized!");
}

void InitTransformComponent(TransformComponent *transformComponent, vec2 position)
{
    transformComponent->position = position;
    //printf("\n---Transform Component Initialized!");
}

void InitHealthComponent(HealthComponent *healthComponent, int maxLives, int startingLives, float maxHealth, float startingHealth, void (*ChangeHealth)(HealthComponent *selfComponent, float amount), void (*Die)(struct Component *selfComponent, struct Game *game))
{
    healthComponent->maxHealth = maxHealth;
    healthComponent->currentHealth = startingHealth;
    healthComponent->ChangeHealth = ChangeHealth;
    healthComponent->Die = Die;
    healthComponent->maxLives = maxLives;
    healthComponent->currentLives = startingLives;

    printf("\n---Health Component Initialized!");
}

void InitUIComponent(struct Component *selfComponent, void (*OnClick)(struct Component *selfComponent, struct Game *game), void (*OnHoverStart)(struct Component *selfComponent), void (*OnHoverEnd)(struct Component *selfComponent))
{
    UIComponent *buttonComponent = (UIComponent *)selfComponent->data;
    buttonComponent->OnClick = OnClick;
    buttonComponent->OnHoverStart = OnHoverStart;
    buttonComponent->OnHoverEnd = OnHoverEnd;
    printf("\n---Button Component Initialized!");
}

void InitPhysicsComponent(struct Component *selfComponent, void (*Collide)(struct Component *selfComponent, struct Component *otherComponent))
{
    PhysicsComponent *physicsComponent = (PhysicsComponent *)selfComponent->data;
    RenderComponent *renderComponent = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);
    if (renderComponent == NULL)
        physicsComponent->colliderRadius = 10;
    else
        physicsComponent->colliderRadius = renderComponent->sprite.spriteRect.w * 0.5f;

    physicsComponent->canCollide = true;
    physicsComponent->Collide = Collide;

    physicsComponent->collisionsBlockTime = 0.5f;
    physicsComponent->collisionsBlockTimeElapsed = 0;

    //COLLISION MASKS ORDER: EnemyBullet, PlayerBullet, Enemy, Player -> TODO: use the enum to make the bitmask.
    switch (selfComponent->owner->type)
    {
    case Player:
        physicsComponent->layersBitmask = ENEMY_COLLISION_LAYER | ENEMYBULLET_COLLISION_LAYER;
        physicsComponent->selfLayer = PLAYER_COLLISION_LAYER;
        break;
    case Enemy:
        physicsComponent->layersBitmask = PLAYER_COLLISION_LAYER | PLAYERBULLET_COLLISION_LAYER;
        physicsComponent->selfLayer = ENEMYBULLET_COLLISION_LAYER;
        break;
    case PlayerBullet:
        physicsComponent->layersBitmask = ENEMY_COLLISION_LAYER;
        physicsComponent->selfLayer = PLAYERBULLET_COLLISION_LAYER;
        break;
    case EnemyBullet:
        physicsComponent->layersBitmask = PLAYER_COLLISION_LAYER;
        physicsComponent->selfLayer = ENEMYBULLET_COLLISION_LAYER;
        break;
    }

    printf("\n---Physics Component Initialized!");
}

void InitRenderComponent(RenderComponent *renderComponent, struct GFXEngine *engine, SpriteType spriteType)
{
    renderComponent->renderer = engine->renderer;
    renderComponent->sprite = GetSprite(engine, spriteType);
    // printf("\n\n-------------------------------------------------\n\n");
    // printf("\n\nOR H: %d | OR W: %d | OR X: %d | OR Y: %d", renderComponent->sprite.originRect.h, renderComponent->sprite.originRect.w, renderComponent->sprite.originRect.x, renderComponent->sprite.originRect.y);
    // printf("\n\nSR H: %d | SR W: %d | SR X: %d | SR Y: %d", renderComponent->sprite.spriteRect.h, renderComponent->sprite.spriteRect.w, renderComponent->sprite.spriteRect.x, renderComponent->sprite.spriteRect.y);
    printf("\n---Render Component Initialized!");
}

void InitAnimatorComponent(struct Component *selfComponent, struct GFXEngine *engine, AnimationType animationType, float frameDuration, void (*SetAnimation)(struct Component *selfComponent, AnimationType type, float frameDuration))
{
    AnimatorComponent *animatorComponent = (AnimatorComponent *)selfComponent->data;
    animatorComponent->engine = engine;
    animatorComponent->SetAnimation = SetAnimation;
    animatorComponent->SetAnimation(selfComponent, animationType, frameDuration);
    // printf("AF Width: %d | AF Height: %d\n", animatorComponent->currentAnimation.frameWidth, animatorComponent->currentAnimation.frameHeight);
    printf("\n---Animator Component Initialized!");
}

void InitShootComponent(ShootComponent *selfComponent, float shootCooldown, EntityType bulletType)
{
    selfComponent->shootCooldown = shootCooldown;
    selfComponent->shootCooldownElapsed = 0;
    selfComponent->bulletType = bulletType;
}

void InitAudioComponent(struct Component *selfComponent, struct AudioEngine *engine, AudioType audioType, int loops, void (*SetAudio)(struct Component *selfComponent, AudioType type, int loops), void (*PlayAudio)(struct Component *selfComponent, struct Game *game))
{
    AudioComponent *ac = (AudioComponent *)selfComponent->data;
    ac->engine = engine;
    ac->SetAudio = SetAudio;
    ac->SetAudio(selfComponent, audioType, loops);
    ac->PlayAudio = PlayAudio;
}

void InitTextComponent(TextComponent *textComponent, char *startingText, FontType fontType, int size)
{
    textComponent->fontType = fontType;
    textComponent->size = size;
    textComponent->text = startingText;
}