#include "timed_behaviour_component.h"

void SpawnPowerupBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);
        int powerupType = GetRandomIntBetween(AttackPowerup, SpeedPowerup);
        struct Entity *powerup = Dequeue(game->engine->poolsEngine, powerupType);
        SetPosition(GetComponentData(powerup, TransformC), vec2_new(GetRandomFloatBetween(30.0f, 620.0f), tc->position.y));
        SetEntityActiveStatus(powerup, true);
        Enqueue(game->engine->poolsEngine, powerup);

        struct BattleLevelData *bld = game->levelData;
        tbc->time = bld->powerupSpawnTimer + GetRandomFloatBetween(bld->powerupSpawnTimerChanger, -bld->powerupSpawnTimerChanger);
    }
}

void DetachSpeedPowerupBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct BattleLevelData *bld = game->levelData;
        bld->playerSpeed /= bld->speedPowerupBonus;
        bld->powerupUI->text = "";
        MarkComponentToDestroy(selfComponent, game->engine->ECS);
    }
}

void DetachAttackPowerupBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct BattleLevelData *bld = game->levelData;
        bld->playerBulletsDamage /= bld->attackPowerupBonus;
        bld->powerupUI->text = "";
        MarkComponentToDestroy(selfComponent, game->engine->ECS);
    }
}

void SpawnIslandBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);
        struct Entity *island = Dequeue(game->engine->poolsEngine, Image);
        SetPosition(GetComponentData(island, TransformC), vec2_new(GetRandomFloatBetween(30.0f, 620.0f), tc->position.y));
        SetEntityActiveStatus(island, true);

        //pick a random island sprite
        int randomIslandSprite = GetRandomIntBetween(Island3S, Island1S);
        InitRenderComponent(GetComponent(island, RenderC), game->engine->GfxEngine, randomIslandSprite);
        Enqueue(game->engine->poolsEngine, island);

        struct BattleLevelData *bld = game->levelData;
        tbc->time = bld->islandSpawnTimer + GetRandomFloatBetween(bld->islandSpawnTimerChanger, -bld->islandSpawnTimerChanger);
    }
}

void SpawnEnemyBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);
        struct Entity *enemy = Dequeue(game->engine->poolsEngine, EnemyAirplane);
        SetPosition(GetComponentData(enemy, TransformC), vec2_new(GetRandomFloatBetween(15.0f, 625.0f), tc->position.y));
        int randomEnemyAnimation = GetRandomIntBetween(Enemy4A, Enemy1A);
        struct Component *c = GetComponent(enemy, AnimatorC);
        struct AnimatorComponent *ac = c->data;
        ac->SetAnimation(c, randomEnemyAnimation, 0);
        struct HealthComponent *hc = GetComponentData(enemy, HealthC);
        hc->currentHealth = hc->maxHealth;
        struct MovementComponent *mc = GetComponentData(enemy, MovementC);
        mc->velocity.x = 0;
        SetEntityActiveStatus(enemy, true);
        Enqueue(game->engine->poolsEngine, enemy);

        struct BattleLevelData *bld = game->levelData;
        tbc->time = bld->enemySpawnTimer + GetRandomFloatBetween(bld->enemySpawnTimerChanger, -bld->enemySpawnTimerChanger);

        //if the ondulation behaviour was active, turn it off
        struct Component *ondulationBehaviour = GetComponent(enemy, TimedBehaviourC);
        SetComponentActiveStatus(ondulationBehaviour, false);

        //now, if the chance is good, activate it
        int velocityChangeExtraction = GetRandomInt(100);
        if (velocityChangeExtraction <= bld->enemyOndulationChance)
        {
            struct TimedBehaviourComponent *tbc = ondulationBehaviour->data;
            tbc->elapsedTime = 0;
            tbc->currentRepetitions = 0;
            SetComponentActiveStatus(ondulationBehaviour, true);
        }
    }
}

void ToggleEntityAfterBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        SetEntityActiveStatus(selfComponent->owner, !selfComponent->owner->active);
    }
}

void ReleaseSmokeBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct HealthComponent *hc = GetComponentData(selfComponent->owner, HealthC);
        {
            if (hc->currentHealth <= hc->maxHealth * 0.25f)
            {
                struct TransformComponent *transformComponent = GetComponentData(selfComponent->owner, TransformC);

                //release two smoke particles behind the plane...
                struct Entity *particle = Dequeue(game->engine->poolsEngine, SmokeParticle);
                struct TransformComponent *particleTransformComponent = GetComponentData(particle, TransformC);
                particleTransformComponent->position = vec2_new(transformComponent->position.x - 15, transformComponent->position.y + 10);
                SetEntityActiveStatus(particle, true);
                Enqueue(game->engine->poolsEngine, particle);
                particle = Dequeue(game->engine->poolsEngine, SmokeParticle);
                particleTransformComponent = GetComponentData(particle, TransformC);
                particleTransformComponent->position = vec2_new(transformComponent->position.x + 15, transformComponent->position.y + 10);
                SetEntityActiveStatus(particle, true);
                Enqueue(game->engine->poolsEngine, particle);
            }
        }
    }
}

void SpawnBossBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct BattleLevelData *bld = game->levelData;
        SetEntityActiveStatus(bld->boss, true);
        bld->enemySpawnTimer = 25.0f;
        bld->enemyOndulationChance = -1;
        bld->maxEnemySquadronSize = 3;
        bld->squadronSpawnTimer = 25.0f;
        MarkComponentToDestroy(selfComponent, game->engine->ECS);
    }
}

void BossSwitchShootBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct BattleLevelData *bld = game->levelData;
        struct Component *mc = GetComponent(selfComponent->owner, ShootC);
        if (mc->behaviour == BossDoubleShootBehaviour)
        {
            mc->behaviour = BossTripleShootBehaviour;
            struct ShootComponent *sc = mc->data;
            sc->shootCooldown = bld->bossTripleShootInterval;
            tbc->time = bld->bossTripleShootSwitch;
        }
        else if (mc->behaviour == BossTripleShootBehaviour)
        {
            mc->behaviour = BossDoubleShootBehaviour;
            struct ShootComponent *sc = mc->data;
            sc->shootCooldown = bld->bossDoubleShootInterval;
            tbc->time = bld->bossDoubleShootSwitch;
        }
    }
}

void SpawnEnemySquadronBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);
        struct BattleLevelData *bld = game->levelData;
        int enemies = GetRandomIntBetween(bld->maxEnemySquadronSize, bld->minEnemySquadronSize);
        float y = tc->position.y;
        float x = GetRandomFloatBetween(15.0f, 625.0f);
        int xDirection = 1;

        for (int i = 0; i < enemies; i++)
        {
            struct Entity *enemy = Dequeue(game->engine->poolsEngine, EnemyAirplane);

            //if the ondulation behaviour was active, turn it off, we don't want the squadrons to ondulate
            struct MovementComponent *mc = GetComponentData(enemy, MovementC);
            mc->velocity.x = 0;
            struct Component *ondulationBehaviour = GetComponent(enemy, TimedBehaviourC);
            SetComponentActiveStatus(ondulationBehaviour, false);

            SetPosition(GetComponentData(enemy, TransformC), vec2_new(x, y));
            int randomEnemyAnimation = GetRandomIntBetween(Enemy4A, Enemy1A);
            struct Component *c = GetComponent(enemy, AnimatorC);
            struct AnimatorComponent *ac = c->data;
            ac->SetAnimation(c, randomEnemyAnimation, 0);
            struct HealthComponent *hc = GetComponentData(enemy, HealthC);
            hc->currentHealth = hc->maxHealth;
            SetEntityActiveStatus(enemy, true);
            Enqueue(game->engine->poolsEngine, enemy);
            y -= 45;
            if (x + 45 >= 625 || x - 45 <= 15)
                xDirection = -xDirection;
            x += 45 * xDirection;
        }
        tbc->time = bld->squadronSpawnTimer;
    }
}

void BossExplosionBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        tbc->currentRepetitions++;

        //make an explosion sound every 2 particles spawned
        if (tbc->currentRepetitions % 2 == 0)
        {
            struct Entity *audioEmitter = Dequeue(game->engine->poolsEngine, AudioEmitter);
            struct Component *c = GetComponent(audioEmitter, AudioC);
            struct AudioComponent *audioComponent = c->data;
            struct TimedBehaviourComponent *tbc = GetComponentData(audioEmitter, TimedBehaviourC);
            tbc->time = 1.2f;
            audioComponent->SetAudio(c, Explosion2SFX, 0);
            SetEntityActiveStatus(audioEmitter, true);
            Enqueue(game->engine->poolsEngine, audioEmitter);
            audioComponent->PlayAudio(c, game);
        }

        struct TransformComponent *transformComponent = GetComponentData(selfComponent->owner, TransformC);

        struct Entity *particle = Dequeue(game->engine->poolsEngine, ExplosionParticle);
        struct TransformComponent *particleTransformComponent = GetComponentData(particle, TransformC);
        float randomX = GetRandomFloatBetween(-50.0f, 50.0f);
        float randomY = GetRandomFloatBetween(-50.0f, 50.0f);
        particleTransformComponent->position = vec2_new(transformComponent->position.x + randomX, transformComponent->position.y + randomY);
        SetEntityActiveStatus(particle, true);
        Enqueue(game->engine->poolsEngine, particle);

        if (tbc->currentRepetitions == tbc->repetitions)
        {
            struct BattleLevelData *bld = game->levelData;

            //increase score
            bld->score += 500;
            sprintf(bld->scoreToString, "%d", bld->score);
            bld->scoreUI->text = bld->scoreToString;

            struct Component *c = AddComponent(bld->player, TimedBehaviourC, GoToMainMenuAfter, game->engine->ECS);
            InitTimedBehaviourComponent(c->data, 1, 3.0f, NULL);
            c = GetComponent(selfComponent->owner, RenderC);
            SetComponentActiveStatus(c, false);
            MarkComponentToDestroy(selfComponent, game->engine->ECS);
        }
    }
}

void ScoreDistanceBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
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
    struct TimedBehaviourComponent *tbc = selfComponent->data;

    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct MovementComponent *mc = GetComponentData(selfComponent->owner, MovementC);

        if (mc->velocity.x == 0)
            mc->velocity.x = 1;

        mc->velocity.x = -mc->velocity.x;

        if (tbc->currentRepetitions >= tbc->repetitions)
        {
            tbc->currentRepetitions = 0;
            //mc->velocity.x = 0;
            SetComponentActiveStatus(selfComponent, false);
        }
    }
}

void PlayerTimedRespawnBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        struct Component *c = GetComponent(selfComponent->owner, HealthC);
        struct HealthComponent *hc = c->data;
        hc->currentHealth = hc->maxHealth;
        c->active = true;
        MarkComponentToDestroy(selfComponent, game->engine->ECS);
    }
}

void SplashScreenBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        game->sceneToLoad = MainMenuScene;
    }
}

void GoToMainMenuAfter(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        tbc->elapsedTime = 0;
        game->sceneToLoad = MainMenuScene;
        struct BattleLevelData *bld = game->levelData;
        aiv_vector_destroy(bld->lives);

        //save high score, if it is beaten
        FILE *fp = fopen("resources/assets/scores/high_score.txt", "r");
        char scoreBuffer[5];
        fgets(scoreBuffer, 5, fp);
        printf("\nCurrent High Score: %s", scoreBuffer);

        int currentHighScore = atoi(scoreBuffer);
        fclose(fp);
        if (bld->score > currentHighScore)
        {
            fp = fopen("resources/assets/scores/high_score.txt", "w");
            fputs(bld->scoreToString, fp);
            printf("\nOverwriting high score with new score %s", scoreBuffer);
        }
        fclose(fp);

        MarkComponentToDestroy(selfComponent, game->engine->ECS);
    }
}

void AlphaBlendingBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct TimedBehaviourComponent *tbc = selfComponent->data;

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

    struct RenderComponent *rc = GetComponentData(selfComponent->owner, RenderC);
    tbc->elapsedTime += game->engine->time->deltaTimeInSeconds;
    if (tbc->elapsedTime >= tbc->time)
    {
        SDL_SetTextureAlphaMod(rc->sprite.texture, 255);
        void *ptr = aiv_vector_remove_at(tbc->customArgs, aiv_vector_size(tbc->customArgs) - 1);
        free(ptr);
        aiv_vector_destroy(tbc->customArgs);
        MarkComponentToDestroy(selfComponent, game->engine->ECS);
        return;
    }

    SDL_SetTextureAlphaMod(rc->sprite.texture, *currentAlpha);
}

void InitTimedBehaviourComponent(struct TimedBehaviourComponent *timedBehaviourComponent, int repetitions, float time, aiv_vector *customArgs)
{
    timedBehaviourComponent->repetitions = repetitions;
    timedBehaviourComponent->currentRepetitions = 0;
    timedBehaviourComponent->time = time;
    timedBehaviourComponent->elapsedTime = 0;
    timedBehaviourComponent->customArgs = customArgs;
}