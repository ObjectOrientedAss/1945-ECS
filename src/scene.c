#include "scene.h"

//DEFINE THE BASIC COMPONENTS FOR EACH ENTITY AND THEIR SYSTEM BEHAVIOUR
void AddComponents(struct Entity *entity)
{
    switch (entity->type)
    {
    case Airplane:
        AddComponent(entity, TransformC, NULL);
        if (entity->family == PlayerAirplane)
        {
            AddComponent(entity, TimedBehaviourC, ReleaseSmokeBehaviour);
            AddComponent(entity, InputC, InputBehaviour);
            AddComponent(entity, MovementC, PlayerMovementBehaviour);
            AddComponent(entity, ShootC, PlayerShootBehaviour);
        }
        else if (entity->family == EnemyAirplane)
        {
            AddComponent(entity, TimedBehaviourC, OndulationBehaviour);
            AddComponent(entity, MovementC, AutomatedMovementBehaviour);
            AddComponent(entity, ShootC, EnemyShootBehaviour);
        }
        else
        {
            AddComponent(entity, TimedBehaviourC, BossSwitchShootBehaviour);
            AddComponent(entity, MovementC, BossEntranceMovementBehaviour);
            AddComponent(entity, ShootC, BossDoubleShootBehaviour);
        }
        AddComponent(entity, PhysicsC, PhysicsBehaviour);
        AddComponent(entity, HealthC, HealthBehaviour);
        AddComponent(entity, AnimatorC, AnimatorBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;

    case Bullet:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, MovementC, AutomatedMovementBehaviour);
        AddComponent(entity, PhysicsC, PhysicsBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;

    case UI:
        switch (entity->family)
        {
        case Button:
            AddComponent(entity, TransformC, NULL);
            AddComponent(entity, UIC, UIBehaviour);
            AddComponent(entity, RenderC, RenderBehaviour);
            break;
        case Text:
            AddComponent(entity, TransformC, NULL);
            AddComponent(entity, TextC, NULL);
            AddComponent(entity, RenderC, FontRenderBehaviour);
            break;
        case Image:
            AddComponent(entity, TransformC, NULL);
            AddComponent(entity, RenderC, RenderBehaviour);
            break;
        }
        break;

    case Particle:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, TimedBehaviourC, ToggleEntityAfterBehaviour);
        AddComponent(entity, MovementC, ParticleMovementBehaviour);
        AddComponent(entity, AnimatorC, AnimatorBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;

    case Audio:
        AddComponent(entity, AudioC, NULL);
        AddComponent(entity, TimedBehaviourC, ToggleEntityAfterBehaviour);
        break;

    case Powerup:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, MovementC, AutomatedMovementBehaviour);
        AddComponent(entity, PhysicsC, PhysicsBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;
    }
}

void LoadMainMenuScene(struct Game *game)
{
    //free(game->levelData);
    struct MainMenuLevelData *mmld = (struct MainMenuLevelData *)calloc(1, sizeof(struct MainMenuLevelData));
    game->levelData = mmld;

    struct Entity *backGround = CreateEntity(UI, Image, true, game->ECS);
    AddComponents(backGround);
    InitTransformComponent(GetComponentData(backGround, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f));
    InitRenderComponent(GetComponentData(backGround, RenderC), game->engine->GfxEngine, TitleS);

    struct Entity *buttonStart = CreateEntity(UI, Button, true, game->ECS);
    AddComponents(buttonStart);
    InitTransformComponent(GetComponentData(buttonStart, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f + 75, game->engine->GfxEngine->windowHeight * 0.5f + 35));
    InitUIComponent(GetComponent(buttonStart, UIC), StartGameClick, ButtonHoverStart, ButtonHoverEnd);
    InitRenderComponent(GetComponentData(buttonStart, RenderC), game->engine->GfxEngine, StartButtonS);

    struct Entity *buttonQuit = CreateEntity(UI, Button, true, game->ECS);
    AddComponents(buttonQuit);
    InitTransformComponent(GetComponentData(buttonQuit, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f + 75, game->engine->GfxEngine->windowHeight * 0.5f + 90));
    InitUIComponent(GetComponent(buttonQuit, UIC), QuitGameClick, ButtonHoverStart, ButtonHoverEnd);
    InitRenderComponent(GetComponentData(buttonQuit, RenderC), game->engine->GfxEngine, QuitButtonS);

    struct Entity *audioEmitter = CreateEntity(Audio, AudioEmitter, true, game->ECS);
    AddComponents(audioEmitter);
    InitAudioComponent(GetComponent(audioEmitter, AudioC), game->engine->audioEngine, BackgroundMusic, -1, SetAudio, PlayAudio);
    Mix_VolumeMusic(5);
    struct AudioComponent *audioComponent = GetComponentData(audioEmitter, AudioC);
    audioComponent->PlayAudio(GetComponent(audioEmitter, AudioC), game);
}

void LoadGameScene(struct Game *game)
{
    struct BattleLevelData *battleLevelData = (struct BattleLevelData *)calloc(1, sizeof(struct BattleLevelData));

    battleLevelData->playerMaxLife = 100.0f;
    battleLevelData->playerBulletsDamage = 50.0f;
    battleLevelData->playerShootCooldown = 0.2f;
    battleLevelData->playerMaxLives = 5;
    battleLevelData->playerSpeed = 0.3f;

    battleLevelData->enemiesShootCooldown = 0.5f;
    battleLevelData->enemyBulletsDamage = 10.0f;
    battleLevelData->enemySpawnTimer = 0.45f;
    battleLevelData->enemySpawnTimerChanger = 0.3f;
    battleLevelData->enemyLife = 100.0f;
    battleLevelData->enemySpeed = 0.2f;

    battleLevelData->bulletSpeed = 0.5f;
    battleLevelData->islandSpawnTimer = 1.5f;
    battleLevelData->islandSpawnTimerChanger = 0.3f;
    battleLevelData->environmentSpeed = 0.1f;

    battleLevelData->enemyKilledScore = 5;
    battleLevelData->increaseScoreTimer = 0.3f;
    battleLevelData->increaseScore = 1;

    battleLevelData->minEnemySquadronSize = 3;
    battleLevelData->maxEnemySquadronSize = 5;
    battleLevelData->squadronSpawnTimer = 5.0f;
    battleLevelData->enemyOndulationChance = 50;

    battleLevelData->powerupSpawnTimer = 16.0f;
    battleLevelData->powerupSpawnTimerChanger = 5.0f;
    battleLevelData->playerSuicideDamage = 50.0f;

    battleLevelData->powerupDuration = 10.0f;
    battleLevelData->attackPowerupBonus = 2;
    battleLevelData->speedPowerupBonus = 25;

    battleLevelData->lifePowerupSpawnChance = 5.0f;
    battleLevelData->bossBulletsDamage = 20.0f;
    battleLevelData->bossBulletsSpeed = 0.4f;
    battleLevelData->bossSpeed = 0.1f;
    battleLevelData->bossHealth = 15000.0f;
    battleLevelData->bossSpawnTimer = 60.0f;
    battleLevelData->bossDoubleShootInterval = 0.4f;
    battleLevelData->bossTripleShootInterval = 0.3f;
    battleLevelData->bossDoubleShootSwitch = 5.0f;
    battleLevelData->bossTripleShootSwitch = 1.8f;

    //CREATE AND INITIALIZE THE GAME SCENE MANAGER
    struct Entity *sceneManager = CreateEntity(NoType, NoFamily, true, game->ECS);
    struct Component *c = NULL;
    c = AddComponent(sceneManager, TransformC, NULL);
    InitTransformComponent(c->data, vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, -40));
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnEnemyBehaviour);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->enemySpawnTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnIslandBehaviour);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->islandSpawnTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, ScoreDistanceBehaviour);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->increaseScoreTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnEnemySquadronBehaviour);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->squadronSpawnTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnPowerupBehaviour);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->powerupSpawnTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnBossBehaviour);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->bossSpawnTimer, NULL);
    //------------------------

    //CREATE AND INITIALIZE THE WATER BACKGROUNDS TO SWAP
    printf("\nCreating water...");
    for (int i = 0; i < 2; i++)
    {
        struct Entity *waterBackground = CreateEntity(UI, Image, true, game->ECS);
        AddComponents(waterBackground);
        AddComponent(waterBackground, MovementC, WaterMovementBehaviour);
        if (i == 0)
            InitTransformComponent(GetComponentData(waterBackground, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f));
        else
            InitTransformComponent(GetComponentData(waterBackground, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, -game->engine->GfxEngine->windowHeight * 0.5f));
        InitMovementComponent(GetComponentData(waterBackground, MovementC), vec2_new(0, 1), battleLevelData->environmentSpeed);
        InitRenderComponent(GetComponentData(waterBackground, RenderC), game->engine->GfxEngine, WaterS);
    }

    //CREATE AND INITIALIZE THE ISLANDS POOL
    printf("\nCreating islands...");
    for (int i = 0; i < 15; i++)
    {
        struct Entity *island = CreateEntity(UI, Image, false, game->ECS);
        AddComponents(island);
        AddComponent(island, MovementC, AutomatedMovementBehaviour);
        InitTransformComponent(GetComponentData(island, TransformC), vec2_new(-100, -100));
        InitMovementComponent(GetComponentData(island, MovementC), vec2_new(0, 1), battleLevelData->environmentSpeed);
        InitRenderComponent(GetComponentData(island, RenderC), game->engine->GfxEngine, Island1S);
        Enqueue(game->engine->poolsEngine, island);
    }

    printf("\nCreating powerups...");
    //CREATE AND INITIALIZE THE ATTACK POWERUP POOL
    for (int i = 0; i < 2; i++)
    {
        struct Entity *powerup = CreateEntity(Powerup, AttackPowerup, false, game->ECS);
        AddComponents(powerup);
        InitTransformComponent(GetComponentData(powerup, TransformC), vec2_new(-100, -100));
        InitMovementComponent(GetComponentData(powerup, MovementC), vec2_new(0, 1), 0.15f);
        InitRenderComponent(GetComponentData(powerup, RenderC), game->engine->GfxEngine, AttackPowerupS);
        InitPhysicsComponent(GetComponent(powerup, PhysicsC), Collide);
        Enqueue(game->engine->poolsEngine, powerup);
    }

    //CREATE AND INITIALIZE THE SPEED POWERUP POOL
    for (int i = 0; i < 2; i++)
    {
        struct Entity *powerup = CreateEntity(Powerup, SpeedPowerup, false, game->ECS);
        AddComponents(powerup);
        InitTransformComponent(GetComponentData(powerup, TransformC), vec2_new(-100, -100));
        InitMovementComponent(GetComponentData(powerup, MovementC), vec2_new(0, 1), 0.15f);
        InitRenderComponent(GetComponentData(powerup, RenderC), game->engine->GfxEngine, SpeedPowerupS);
        InitPhysicsComponent(GetComponent(powerup, PhysicsC), Collide);
        Enqueue(game->engine->poolsEngine, powerup);
    }

    //CREATE AND INITIALIZE THE LIFE POWERUP POOL
    for (int i = 0; i < 3; i++)
    {
        struct Entity *powerup = CreateEntity(Powerup, LifePowerup, false, game->ECS);
        AddComponents(powerup);
        InitTransformComponent(GetComponentData(powerup, TransformC), vec2_new(-100, -100));
        InitMovementComponent(GetComponentData(powerup, MovementC), vec2_new(0, 1), 0.15f);
        InitRenderComponent(GetComponentData(powerup, RenderC), game->engine->GfxEngine, LifePowerupS);
        InitPhysicsComponent(GetComponent(powerup, PhysicsC), Collide);
        Enqueue(game->engine->poolsEngine, powerup);
    }

    printf("\nCreating bullets...");
    //CREATE AND INITIALIZE THE ENEMY BULLETS POOL
    for (int i = 0; i < 50; i++)
    {
        struct Entity *enemyBullet = CreateEntity(Bullet, EnemyBullet, false, game->ECS);
        AddComponents(enemyBullet);
        InitTransformComponent(GetComponentData(enemyBullet, TransformC), vec2_new(-100, -100));
        InitMovementComponent(GetComponentData(enemyBullet, MovementC), vec2_new(0, 1), battleLevelData->bulletSpeed);
        InitRenderComponent(GetComponentData(enemyBullet, RenderC), game->engine->GfxEngine, EnemyBullet1S);
        InitPhysicsComponent(GetComponent(enemyBullet, PhysicsC), Collide);
        Enqueue(game->engine->poolsEngine, enemyBullet);
    }

    printf("\nCreating more bullets...");
    //CREATE AND INITIALIZE THE ENEMY BULLETS POOL
    for (int i = 0; i < 50; i++)
    {
        struct Entity *enemyBullet = CreateEntity(Bullet, BossBullet, false, game->ECS);
        AddComponents(enemyBullet);
        InitTransformComponent(GetComponentData(enemyBullet, TransformC), vec2_new(-100, -100));
        InitMovementComponent(GetComponentData(enemyBullet, MovementC), vec2_new(0, 1), battleLevelData->bossBulletsSpeed);
        InitRenderComponent(GetComponentData(enemyBullet, RenderC), game->engine->GfxEngine, EnemyBullet2S);
        InitPhysicsComponent(GetComponent(enemyBullet, PhysicsC), Collide);
        Enqueue(game->engine->poolsEngine, enemyBullet);
    }

    printf("\nBullets are never enough you know?");
    //CREATE AND INITIALIZE THE PLAYER BULLETS POOL
    for (int i = 0; i < 15; i++)
    {
        struct Entity *playerBullet = CreateEntity(Bullet, PlayerBullet, false, game->ECS);
        AddComponents(playerBullet);
        InitTransformComponent(GetComponentData(playerBullet, TransformC), vec2_new(-200, -200));
        InitMovementComponent(GetComponentData(playerBullet, MovementC), vec2_new(0, -1), battleLevelData->bulletSpeed);
        InitRenderComponent(GetComponentData(playerBullet, RenderC), game->engine->GfxEngine, PlayerBulletS);
        InitPhysicsComponent(GetComponent(playerBullet, PhysicsC), Collide);
        Enqueue(game->engine->poolsEngine, playerBullet);
    }

    printf("\nCreating enemies...");
    //CREATE AND INITIALIZE THE ENEMIES POOL
    for (int i = 0; i < 20; i++)
    {
        struct Entity *enemy = CreateEntity(Airplane, EnemyAirplane, false, game->ECS);
        AddComponents(enemy);
        InitTransformComponent(GetComponentData(enemy, TransformC), vec2_new(-100, -100));
        InitHealthComponent(GetComponentData(enemy, HealthC), 1, 1, battleLevelData->enemyLife, battleLevelData->enemyLife, ChangeHealth, EnemyDeath);
        InitMovementComponent(GetComponentData(enemy, MovementC), vec2_new(0, 1), battleLevelData->enemySpeed);
        InitRenderComponent(GetComponentData(enemy, RenderC), game->engine->GfxEngine, NullSprite);
        InitAnimatorComponent(GetComponent(enemy, AnimatorC), game->engine->GfxEngine, Enemy1A, 0.3f, SetAnimation);
        InitPhysicsComponent(GetComponent(enemy, PhysicsC), Collide);
        InitShootComponent(GetComponentData(enemy, ShootC), battleLevelData->enemiesShootCooldown, EnemyBullet);
        InitTimedBehaviourComponent(GetComponentData(enemy, TimedBehaviourC), 10, 0.3f, NULL); //ondulation behaviour
        Enqueue(game->engine->poolsEngine, enemy);
    }

    printf("\nCreating audio emitters...");
    //CREATE AND INITIALIZE THE AUDIO EMITTERS POOL
    for (int i = 0; i < 20; i++)
    {
        struct Entity *audioEmitter = CreateEntity(Audio, AudioEmitter, false, game->ECS);
        AddComponents(audioEmitter);
        InitAudioComponent(GetComponent(audioEmitter, AudioC), game->engine->audioEngine, BackgroundMusic, -1, SetAudio, PlayAudio);
        InitTimedBehaviourComponent(GetComponentData(audioEmitter, TimedBehaviourC), 1, 0.2f, NULL); //toggle entity after behaviour
        Enqueue(game->engine->poolsEngine, audioEmitter);
    }

    //CREATE AND INITIALIZE THE BOSS
    printf("\nCreating boss... a very bad one.");
    struct Entity *boss = CreateEntity(Airplane, BossAirplane, false, game->ECS);
    AddComponents(boss);
    InitTransformComponent(GetComponentData(boss, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, -200));
    InitHealthComponent(GetComponentData(boss, HealthC), 1, 1, battleLevelData->bossHealth, battleLevelData->bossHealth, ChangeHealth, BossDeath);
    InitMovementComponent(GetComponentData(boss, MovementC), vec2_new(0, 1), battleLevelData->bossSpeed);
    InitRenderComponent(GetComponentData(boss, RenderC), game->engine->GfxEngine, NullSprite);
    InitAnimatorComponent(GetComponent(boss, AnimatorC), game->engine->GfxEngine, BossA, 0.3f, SetAnimation);
    InitPhysicsComponent(GetComponent(boss, PhysicsC), Collide);
    InitShootComponent(GetComponentData(boss, ShootC), battleLevelData->bossDoubleShootInterval, BossBullet);
    InitTimedBehaviourComponent(GetComponentData(boss, TimedBehaviourC), 1, battleLevelData->bossDoubleShootSwitch, NULL); //shoot switch behaviour
    battleLevelData->boss = boss;

    //CREATE AND INITIALIZE THE PLAYER
    printf("\nCreating player...");
    struct Entity *player = CreateEntity(Airplane, PlayerAirplane, true, game->ECS);
    AddComponents(player);
    InitTransformComponent(GetComponentData(player, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight - 100));
    InitHealthComponent(GetComponentData(player, HealthC), battleLevelData->playerMaxLives, battleLevelData->playerMaxLives, battleLevelData->playerMaxLife, battleLevelData->playerMaxLife, ChangeHealth, PlayerDeath);
    InitMovementComponent(GetComponentData(player, MovementC), vec2_new(0, 0), battleLevelData->playerSpeed);
    InitRenderComponent(GetComponentData(player, RenderC), game->engine->GfxEngine, NullSprite);
    InitAnimatorComponent(GetComponent(player, AnimatorC), game->engine->GfxEngine, PlayerA, 0.3f, SetAnimation);
    InitPhysicsComponent(GetComponent(player, PhysicsC), Collide);
    InitShootComponent(GetComponentData(player, ShootC), battleLevelData->playerShootCooldown, PlayerBullet);
    InitTimedBehaviourComponent(GetComponentData(player, TimedBehaviourC), 1, 0.5f, NULL); //release smoke behaviour

    battleLevelData->player = player;

    printf("\nCreating explosion particles...");
    //CREATE AND INITIALIZE THE EXPLOSION PARTICLES POOL
    for (int i = 0; i < 10; i++)
    {
        struct Entity *particle = CreateEntity(Particle, ExplosionParticle, false, game->ECS);
        AddComponents(particle);
        InitTransformComponent(GetComponentData(particle, TransformC), vec2_new(-100, -100));
        InitTimedBehaviourComponent(GetComponentData(particle, TimedBehaviourC), 1, 0.9f, NULL); //toggle entity after behaviour
        InitMovementComponent(GetComponentData(particle, MovementC), vec2_new(0, 0), 0);
        InitRenderComponent(GetComponentData(particle, RenderC), game->engine->GfxEngine, NullSprite);
        InitAnimatorComponent(GetComponent(particle, AnimatorC), game->engine->GfxEngine, EnemyExplosionA, 0.15f, SetAnimation);
        Enqueue(game->engine->poolsEngine, particle);
    }

    printf("\nCreating smoke particles...");
    //CREATE AND INITIALIZE THE SMOKE PARTICLES POOL
    for (int i = 0; i < 15; i++)
    {
        struct Entity *particle = CreateEntity(Particle, SmokeParticle, false, game->ECS);
        AddComponents(particle);
        InitTransformComponent(GetComponentData(particle, TransformC), vec2_new(-100, -100));
        InitTimedBehaviourComponent(GetComponentData(particle, TimedBehaviourC), 1, 1.5f, NULL); //toggle entity after behaviour
        InitMovementComponent(GetComponentData(particle, MovementC), vec2_new(0, 1), 0.3f);
        InitRenderComponent(GetComponentData(particle, RenderC), game->engine->GfxEngine, NullSprite);
        InitAnimatorComponent(GetComponent(particle, AnimatorC), game->engine->GfxEngine, SmokeA, 0.15f, SetAnimation);
        Enqueue(game->engine->poolsEngine, particle);
    }

    //CREATE AND INITIALIZE THE UI
    printf("\nCreating UI...");
    //panel ui
    struct Entity *bottomUI = CreateEntity(UI, Image, true, game->ECS);
    AddComponents(bottomUI);
    InitTransformComponent(GetComponentData(bottomUI, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight - 38));
    InitRenderComponent(GetComponentData(bottomUI, RenderC), game->engine->GfxEngine, BottomS);
    game->levelData = battleLevelData;

    //lives ui
    int x = 35;
    int y = 425;
    battleLevelData->lives = aiv_vector_new_with_cap(battleLevelData->playerMaxLives);
    for (int i = 0; i < battleLevelData->playerMaxLives; i++)
    {
        struct Entity *playerLifeUI = CreateEntity(UI, Image, true, game->ECS);
        AddComponents(playerLifeUI);
        InitTransformComponent(GetComponentData(playerLifeUI, TransformC), vec2_new(x, y));
        InitRenderComponent(GetComponentData(playerLifeUI, RenderC), game->engine->GfxEngine, LifeS);
        aiv_vector_add(battleLevelData->lives, playerLifeUI);
        x += 30;
    }

    //score ui
    struct Entity *scoreUI = CreateEntity(UI, Text, true, game->ECS);
    AddComponents(scoreUI);
    InitTransformComponent(GetComponentData(scoreUI, TransformC), vec2_new(95, 455));
    InitRenderComponent(GetComponentData(scoreUI, RenderC), game->engine->GfxEngine, NullSprite);
    InitTextComponent(GetComponentData(scoreUI, TextC), "0", Haettenschweiler, 18);
    battleLevelData->scoreUI = GetComponentData(scoreUI, TextC);

    //current powerup ui
    struct Entity *powerupUI = CreateEntity(UI, Text, true, game->ECS);
    AddComponents(powerupUI);
    InitTransformComponent(GetComponentData(powerupUI, TransformC), vec2_new(150, 455));
    InitRenderComponent(GetComponentData(powerupUI, RenderC), game->engine->GfxEngine, NullSprite);
    InitTextComponent(GetComponentData(powerupUI, TextC), "", Haettenschweiler, 18);
    battleLevelData->powerupUI = GetComponentData(powerupUI, TextC);
}