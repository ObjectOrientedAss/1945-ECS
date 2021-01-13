#include "scene.h"

//DEFINE THE BASIC COMPONENTS FOR EACH ENTITY AND THEIR SYSTEM BEHAVIOUR
void AddComponents(struct Entity * entity, struct EntityComponentSystem* ECS)
{
    switch (entity->type)
    {
    case Airplane: 
        AddComponent(entity, TransformC, NULL, ECS);
        if (entity->family == PlayerAirplane)
        {
            AddComponent(entity, TimedBehaviourC, ReleaseSmokeBehaviour, ECS);
            AddComponent(entity, InputC, InputBehaviour, ECS);
            AddComponent(entity, MovementC, PlayerMovementBehaviour, ECS);
            AddComponent(entity, ShootC, PlayerShootBehaviour, ECS);
        }
        else if (entity->family == EnemyAirplane)
        {
            AddComponent(entity, TimedBehaviourC, OndulationBehaviour, ECS);
            AddComponent(entity, MovementC, AutomatedMovementBehaviour, ECS);
            AddComponent(entity, ShootC, EnemyShootBehaviour, ECS);
        }
        else
        {
            AddComponent(entity, TimedBehaviourC, BossSwitchShootBehaviour, ECS);
            AddComponent(entity, MovementC, BossEntranceMovementBehaviour, ECS);
            AddComponent(entity, ShootC, BossDoubleShootBehaviour, ECS);
        }
        AddComponent(entity, PhysicsC, PhysicsBehaviour, ECS);
        AddComponent(entity, HealthC, HealthBehaviour, ECS);
        AddComponent(entity, AnimatorC, AnimatorBehaviour, ECS);
        AddComponent(entity, RenderC, RenderBehaviour, ECS);
        break;

    case Bullet:
        AddComponent(entity, TransformC, NULL, ECS);
        AddComponent(entity, MovementC, AutomatedMovementBehaviour, ECS);
        AddComponent(entity, PhysicsC, PhysicsBehaviour, ECS);
        AddComponent(entity, RenderC, RenderBehaviour, ECS);
        break;

    case UI:
        switch (entity->family)
        {
        case Button:
            AddComponent(entity, TransformC, NULL, ECS);
            AddComponent(entity, UIC, UIBehaviour, ECS);
            AddComponent(entity, RenderC, RenderBehaviour, ECS);
            break;
        case Text:
            AddComponent(entity, TransformC, NULL, ECS);
            AddComponent(entity, TextC, NULL, ECS);
            AddComponent(entity, RenderC, FontRenderBehaviour, ECS);
            break;
        case Image:
            AddComponent(entity, TransformC, NULL, ECS);
            AddComponent(entity, RenderC, RenderBehaviour, ECS);
            break;
        }
        break;

    case Particle:
        AddComponent(entity, TransformC, NULL, ECS);
        AddComponent(entity, TimedBehaviourC, ToggleEntityAfterBehaviour, ECS);
        AddComponent(entity, MovementC, ParticleMovementBehaviour, ECS);
        AddComponent(entity, AnimatorC, AnimatorBehaviour, ECS);
        AddComponent(entity, RenderC, RenderBehaviour, ECS);
        break;

    case Audio:
        AddComponent(entity, AudioC, NULL, ECS);
        AddComponent(entity, TimedBehaviourC, ToggleEntityAfterBehaviour, ECS);
        break;

    case Powerup:
        AddComponent(entity, TransformC, NULL, ECS);
        AddComponent(entity, MovementC, AutomatedMovementBehaviour, ECS);
        AddComponent(entity, PhysicsC, PhysicsBehaviour, ECS);
        AddComponent(entity, RenderC, RenderBehaviour, ECS);
        break;
    }
}

void LoadSplashScreenScene(struct Game *game)
{
    struct Entity *backGround = CreateEntity(UI, Image, true, game->engine->ECS);
    AddComponents(backGround, game->engine->ECS);
    InitTransformComponent(GetComponentData(backGround, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f), vec2_new(1, 1));
    InitRenderComponent(GetComponent(backGround, RenderC), game->engine->GfxEngine, SplashS);

    struct Component* c = AddComponent(backGround, TimedBehaviourC, SplashScreenBehaviour, game->engine->ECS);
    InitTimedBehaviourComponent(c->data, 1, 5.0f, NULL);
}

void LoadMainMenuScene(struct Game *game)
{
    struct MainMenuLevelData *mmld = (struct MainMenuLevelData *)calloc(1, sizeof(struct MainMenuLevelData));
    game->levelData = mmld;

    struct Entity *backGround = CreateEntity(UI, Image, true, game->engine->ECS);
    AddComponents(backGround, game->engine->ECS);
    InitTransformComponent(GetComponentData(backGround, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f), vec2_new(1, 1));
    InitRenderComponent(GetComponent(backGround, RenderC), game->engine->GfxEngine, TitleS);

    struct Entity *buttonStart = CreateEntity(UI, Button, true, game->engine->ECS);
    AddComponents(buttonStart, game->engine->ECS);
    InitTransformComponent(GetComponentData(buttonStart, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f + 75, game->engine->GfxEngine->windowHeight * 0.5f + 35), vec2_new(1, 1));
    InitUIComponent(GetComponent(buttonStart, UIC), StartGameClick, ButtonHoverStart, ButtonHoverEnd);
    InitRenderComponent(GetComponent(buttonStart, RenderC), game->engine->GfxEngine, StartButtonS);

    struct Entity *buttonQuit = CreateEntity(UI, Button, true, game->engine->ECS);
    AddComponents(buttonQuit, game->engine->ECS);
    InitTransformComponent(GetComponentData(buttonQuit, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f + 75, game->engine->GfxEngine->windowHeight * 0.5f + 90), vec2_new(1, 1));
    InitUIComponent(GetComponent(buttonQuit, UIC), QuitGameClick, ButtonHoverStart, ButtonHoverEnd);
    InitRenderComponent(GetComponent(buttonQuit, RenderC), game->engine->GfxEngine, QuitButtonS);

    struct Entity *audioEmitter = CreateEntity(Audio, AudioEmitter, true, game->engine->ECS);
    AddComponents(audioEmitter, game->engine->ECS);
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
    battleLevelData->bossHealth = 1000.0f;
    battleLevelData->bossSpawnTimer = 10.0f;
    battleLevelData->bossDoubleShootInterval = 0.5f;
    battleLevelData->bossTripleShootInterval = 0.3f;
    battleLevelData->bossDoubleShootSwitch = 5.0f;
    battleLevelData->bossTripleShootSwitch = 1.8f;

    //CREATE AND INITIALIZE THE GAME SCENE MANAGER
    struct Entity *sceneManager = CreateEntity(NoType, NoFamily, true, game->engine->ECS);
    struct Component *c = NULL;
    c = AddComponent(sceneManager, TransformC, NULL, game->engine->ECS);
    InitTransformComponent(c->data, vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, -40), vec2_new(1, 1));
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnEnemyBehaviour, game->engine->ECS);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->enemySpawnTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnIslandBehaviour, game->engine->ECS);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->islandSpawnTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, ScoreDistanceBehaviour, game->engine->ECS);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->increaseScoreTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnEnemySquadronBehaviour, game->engine->ECS);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->squadronSpawnTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnPowerupBehaviour, game->engine->ECS);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->powerupSpawnTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnBossBehaviour, game->engine->ECS);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->bossSpawnTimer, NULL);
    //------------------------

    //CREATE AND INITIALIZE THE WATER BACKGROUNDS TO SWAP
    printf("\nCreating water...");
    for (int i = 0; i < 2; i++)
    {
        struct Entity *waterBackground = CreateEntity(UI, Image, true, game->engine->ECS);
        AddComponents(waterBackground, game->engine->ECS);
        AddComponent(waterBackground, MovementC, WaterMovementBehaviour, game->engine->ECS);
        if (i == 0)
            InitTransformComponent(GetComponentData(waterBackground, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f), vec2_new(1, 1));
        else
            InitTransformComponent(GetComponentData(waterBackground, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, -game->engine->GfxEngine->windowHeight * 0.5f), vec2_new(1, 1));
        InitMovementComponent(GetComponentData(waterBackground, MovementC), vec2_new(0, 1), battleLevelData->environmentSpeed);
        InitRenderComponent(GetComponent(waterBackground, RenderC), game->engine->GfxEngine, WaterS);
    }

    //CREATE AND INITIALIZE THE ISLANDS POOL
    printf("\nCreating islands...");
    for (int i = 0; i < 15; i++)
    {
        struct Entity *island = CreateEntity(UI, Image, false, game->engine->ECS);
        AddComponents(island, game->engine->ECS);
        AddComponent(island, MovementC, AutomatedMovementBehaviour, game->engine->ECS);
        InitTransformComponent(GetComponentData(island, TransformC), vec2_new(-100, -100), vec2_new(1, 1));
        InitMovementComponent(GetComponentData(island, MovementC), vec2_new(0, 1), battleLevelData->environmentSpeed);
        InitRenderComponent(GetComponent(island, RenderC), game->engine->GfxEngine, Island1S);
        Enqueue(game->engine->poolsEngine, island);
    }

    printf("\nCreating powerups...");
    //CREATE AND INITIALIZE THE ATTACK POWERUP POOL
    for (int i = 0; i < 2; i++)
    {
        struct Entity *powerup = CreateEntity(Powerup, AttackPowerup, false, game->engine->ECS);
        AddComponents(powerup, game->engine->ECS);
        InitTransformComponent(GetComponentData(powerup, TransformC), vec2_new(-100, -100), vec2_new(1, 1));
        InitMovementComponent(GetComponentData(powerup, MovementC), vec2_new(0, 1), 0.15f);
        InitRenderComponent(GetComponent(powerup, RenderC), game->engine->GfxEngine, AttackPowerupS);
        InitPhysicsComponent(GetComponent(powerup, PhysicsC), Collide, false, 0);
        Enqueue(game->engine->poolsEngine, powerup);
    }

    //CREATE AND INITIALIZE THE SPEED POWERUP POOL
    for (int i = 0; i < 2; i++)
    {
        struct Entity *powerup = CreateEntity(Powerup, SpeedPowerup, false, game->engine->ECS);
        AddComponents(powerup, game->engine->ECS);
        InitTransformComponent(GetComponentData(powerup, TransformC), vec2_new(-100, -100), vec2_new(1, 1));
        InitMovementComponent(GetComponentData(powerup, MovementC), vec2_new(0, 1), 0.15f);
        InitRenderComponent(GetComponent(powerup, RenderC), game->engine->GfxEngine, SpeedPowerupS);
        InitPhysicsComponent(GetComponent(powerup, PhysicsC), Collide, false, 0);
        Enqueue(game->engine->poolsEngine, powerup);
    }

    //CREATE AND INITIALIZE THE LIFE POWERUP POOL
    for (int i = 0; i < 3; i++)
    {
        struct Entity *powerup = CreateEntity(Powerup, LifePowerup, false, game->engine->ECS);
        AddComponents(powerup, game->engine->ECS);
        InitTransformComponent(GetComponentData(powerup, TransformC), vec2_new(-100, -100), vec2_new(1, 1));
        InitMovementComponent(GetComponentData(powerup, MovementC), vec2_new(0, 1), 0.15f);
        InitRenderComponent(GetComponent(powerup, RenderC), game->engine->GfxEngine, LifePowerupS);
        InitPhysicsComponent(GetComponent(powerup, PhysicsC), Collide, false, 0);
        Enqueue(game->engine->poolsEngine, powerup);
    }

    printf("\nCreating bullets...");
    //CREATE AND INITIALIZE THE ENEMY BULLETS POOL
    for (int i = 0; i < 50; i++)
    {
        struct Entity *enemyBullet = CreateEntity(Bullet, EnemyBullet, false, game->engine->ECS);
        AddComponents(enemyBullet, game->engine->ECS);
        InitTransformComponent(GetComponentData(enemyBullet, TransformC), vec2_new(-100, -100), vec2_new(1, 1));
        InitMovementComponent(GetComponentData(enemyBullet, MovementC), vec2_new(0, 1), battleLevelData->bulletSpeed);
        InitRenderComponent(GetComponent(enemyBullet, RenderC), game->engine->GfxEngine, EnemyBullet1S);
        InitPhysicsComponent(GetComponent(enemyBullet, PhysicsC), Collide, false, 0);
        Enqueue(game->engine->poolsEngine, enemyBullet);
    }

    printf("\nCreating more bullets...");
    //CREATE AND INITIALIZE THE ENEMY BULLETS POOL
    for (int i = 0; i < 50; i++)
    {
        struct Entity *enemyBullet = CreateEntity(Bullet, BossBullet, false, game->engine->ECS);
        AddComponents(enemyBullet, game->engine->ECS);
        InitTransformComponent(GetComponentData(enemyBullet, TransformC), vec2_new(-100, -100), vec2_new(1, 1));
        InitMovementComponent(GetComponentData(enemyBullet, MovementC), vec2_new(0, 1), battleLevelData->bossBulletsSpeed);
        InitRenderComponent(GetComponent(enemyBullet, RenderC), game->engine->GfxEngine, EnemyBullet2S);
        InitPhysicsComponent(GetComponent(enemyBullet, PhysicsC), Collide, false, 0);
        Enqueue(game->engine->poolsEngine, enemyBullet);
    }

    printf("\nBullets are never enough you know?");
    //CREATE AND INITIALIZE THE PLAYER BULLETS POOL
    for (int i = 0; i < 15; i++)
    {
        struct Entity *playerBullet = CreateEntity(Bullet, PlayerBullet, false, game->engine->ECS);
        AddComponents(playerBullet, game->engine->ECS);
        InitTransformComponent(GetComponentData(playerBullet, TransformC), vec2_new(-200, -200), vec2_new(1, 1));
        InitMovementComponent(GetComponentData(playerBullet, MovementC), vec2_new(0, -1), battleLevelData->bulletSpeed);
        InitRenderComponent(GetComponent(playerBullet, RenderC), game->engine->GfxEngine, PlayerBulletS);
        InitPhysicsComponent(GetComponent(playerBullet, PhysicsC), Collide, false, 0);
        Enqueue(game->engine->poolsEngine, playerBullet);
    }

    printf("\nCreating enemies...");
    //CREATE AND INITIALIZE THE ENEMIES POOL
    for (int i = 0; i < 20; i++)
    {
        struct Entity *enemy = CreateEntity(Airplane, EnemyAirplane, false, game->engine->ECS);
        AddComponents(enemy, game->engine->ECS);
        InitTransformComponent(GetComponentData(enemy, TransformC), vec2_new(-100, -100), vec2_new(1, 1));
        InitHealthComponent(GetComponentData(enemy, HealthC), 1, 1, battleLevelData->enemyLife, battleLevelData->enemyLife, ChangeHealth, EnemyDeath);
        InitMovementComponent(GetComponentData(enemy, MovementC), vec2_new(0, 1), battleLevelData->enemySpeed);
        InitRenderComponent(GetComponent(enemy, RenderC), game->engine->GfxEngine, NullSprite);
        InitAnimatorComponent(GetComponent(enemy, AnimatorC), game->engine->GfxEngine, Enemy1A, 0.3f, SetAnimation);
        InitPhysicsComponent(GetComponent(enemy, PhysicsC), Collide, false, 0);
        InitShootComponent(GetComponentData(enemy, ShootC), battleLevelData->enemiesShootCooldown, EnemyBullet);
        InitTimedBehaviourComponent(GetComponentData(enemy, TimedBehaviourC), 10, 0.3f, NULL); //ondulation behaviour
        Enqueue(game->engine->poolsEngine, enemy);
    }

    printf("\nCreating audio emitters...");
    //CREATE AND INITIALIZE THE AUDIO EMITTERS POOL
    for (int i = 0; i < 20; i++)
    {
        struct Entity *audioEmitter = CreateEntity(Audio, AudioEmitter, false, game->engine->ECS);
        AddComponents(audioEmitter, game->engine->ECS);
        InitAudioComponent(GetComponent(audioEmitter, AudioC), game->engine->audioEngine, BackgroundMusic, -1, SetAudio, PlayAudio);
        InitTimedBehaviourComponent(GetComponentData(audioEmitter, TimedBehaviourC), 1, 0.2f, NULL); //toggle entity after behaviour
        Enqueue(game->engine->poolsEngine, audioEmitter);
    }

    //CREATE AND INITIALIZE THE BOSS
    printf("\nCreating boss... a very bad one.");
    struct Entity *boss = CreateEntity(Airplane, BossAirplane, false, game->engine->ECS);
    AddComponents(boss, game->engine->ECS);
    InitTransformComponent(GetComponentData(boss, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, -200), vec2_new(1, 1));
    InitHealthComponent(GetComponentData(boss, HealthC), 1, 1, battleLevelData->bossHealth, battleLevelData->bossHealth, ChangeHealth, BossDeath);
    InitMovementComponent(GetComponentData(boss, MovementC), vec2_new(0, 1), battleLevelData->bossSpeed);
    InitRenderComponent(GetComponent(boss, RenderC), game->engine->GfxEngine, NullSprite);
    InitAnimatorComponent(GetComponent(boss, AnimatorC), game->engine->GfxEngine, BossA, 0.3f, SetAnimation);
    InitPhysicsComponent(GetComponent(boss, PhysicsC), Collide, false, 0);
    InitShootComponent(GetComponentData(boss, ShootC), battleLevelData->bossDoubleShootInterval, BossBullet);
    InitTimedBehaviourComponent(GetComponentData(boss, TimedBehaviourC), 1, battleLevelData->bossDoubleShootSwitch, NULL); //shoot switch behaviour
    battleLevelData->boss = boss;

    //CREATE AND INITIALIZE THE PLAYER
    printf("\nCreating player...");
    struct Entity *player = CreateEntity(Airplane, PlayerAirplane, true, game->engine->ECS);
    AddComponents(player, game->engine->ECS);
    InitTransformComponent(GetComponentData(player, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight - 100), vec2_new(1, 1));
    InitHealthComponent(GetComponentData(player, HealthC), battleLevelData->playerMaxLives, battleLevelData->playerMaxLives, battleLevelData->playerMaxLife, battleLevelData->playerMaxLife, ChangeHealth, PlayerDeath);
    InitMovementComponent(GetComponentData(player, MovementC), vec2_new(0, 0), battleLevelData->playerSpeed);
    InitRenderComponent(GetComponent(player, RenderC), game->engine->GfxEngine, NullSprite);
    InitAnimatorComponent(GetComponent(player, AnimatorC), game->engine->GfxEngine, PlayerA, 0.3f, SetAnimation);
    InitPhysicsComponent(GetComponent(player, PhysicsC), Collide, false, 0);
    InitShootComponent(GetComponentData(player, ShootC), battleLevelData->playerShootCooldown, PlayerBullet);
    InitTimedBehaviourComponent(GetComponentData(player, TimedBehaviourC), 1, 0.5f, NULL); //release smoke behaviour

    battleLevelData->player = player;

    printf("\nCreating explosion particles...");
    //CREATE AND INITIALIZE THE EXPLOSION PARTICLES POOL
    for (int i = 0; i < 10; i++)
    {
        struct Entity *particle = CreateEntity(Particle, ExplosionParticle, false, game->engine->ECS);
        AddComponents(particle, game->engine->ECS);
        InitTransformComponent(GetComponentData(particle, TransformC), vec2_new(-100, -100), vec2_new(1, 1));
        InitTimedBehaviourComponent(GetComponentData(particle, TimedBehaviourC), 1, 0.9f, NULL); //toggle entity after behaviour
        InitMovementComponent(GetComponentData(particle, MovementC), vec2_new(0, 0), 0);
        InitRenderComponent(GetComponent(particle, RenderC), game->engine->GfxEngine, NullSprite);
        InitAnimatorComponent(GetComponent(particle, AnimatorC), game->engine->GfxEngine, EnemyExplosionA, 0.15f, SetAnimation);
        Enqueue(game->engine->poolsEngine, particle);
    }

    printf("\nCreating smoke particles...");
    //CREATE AND INITIALIZE THE SMOKE PARTICLES POOL
    for (int i = 0; i < 15; i++)
    {
        struct Entity *particle = CreateEntity(Particle, SmokeParticle, false, game->engine->ECS);
        AddComponents(particle, game->engine->ECS);
        InitTransformComponent(GetComponentData(particle, TransformC), vec2_new(-100, -100), vec2_new(1, 1));
        InitTimedBehaviourComponent(GetComponentData(particle, TimedBehaviourC), 1, 1.5f, NULL); //toggle entity after behaviour
        InitMovementComponent(GetComponentData(particle, MovementC), vec2_new(0, 1), 0.3f);
        InitRenderComponent(GetComponent(particle, RenderC), game->engine->GfxEngine, NullSprite);
        InitAnimatorComponent(GetComponent(particle, AnimatorC), game->engine->GfxEngine, SmokeA, 0.15f, SetAnimation);
        Enqueue(game->engine->poolsEngine, particle);
    }

    //CREATE AND INITIALIZE THE UI
    printf("\nCreating UI...");
    //panel ui
    struct Entity *bottomUI = CreateEntity(UI, Image, true, game->engine->ECS);
    AddComponents(bottomUI, game->engine->ECS);
    InitTransformComponent(GetComponentData(bottomUI, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight - 38), vec2_new(1, 1));
    InitRenderComponent(GetComponent(bottomUI, RenderC), game->engine->GfxEngine, BottomS);
    game->levelData = battleLevelData;

    //lives ui
    int x = 35;
    int y = 425;
    battleLevelData->lives = aiv_vector_new_with_cap(battleLevelData->playerMaxLives);
    for (int i = 0; i < battleLevelData->playerMaxLives; i++)
    {
        struct Entity *playerLifeUI = CreateEntity(UI, Image, true, game->engine->ECS);
        AddComponents(playerLifeUI, game->engine->ECS);
        InitTransformComponent(GetComponentData(playerLifeUI, TransformC), vec2_new(x, y), vec2_new(1, 1));
        InitRenderComponent(GetComponent(playerLifeUI, RenderC), game->engine->GfxEngine, LifeS);
        aiv_vector_add(battleLevelData->lives, playerLifeUI);
        x += 30;
    }

    //score ui
    struct Entity *scoreUI = CreateEntity(UI, Text, true, game->engine->ECS);
    AddComponents(scoreUI, game->engine->ECS);
    InitTransformComponent(GetComponentData(scoreUI, TransformC), vec2_new(95, 455), vec2_new(1, 1));
    InitRenderComponent(GetComponent(scoreUI, RenderC), game->engine->GfxEngine, NullSprite);
    InitTextComponent(GetComponentData(scoreUI, TextC), "0", Haettenschweiler, 18);
    battleLevelData->scoreUI = GetComponentData(scoreUI, TextC);

    //current powerup ui
    struct Entity *powerupUI = CreateEntity(UI, Text, true, game->engine->ECS);
    AddComponents(powerupUI, game->engine->ECS);
    InitTransformComponent(GetComponentData(powerupUI, TransformC), vec2_new(150, 455), vec2_new(1, 1));
    InitRenderComponent(GetComponent(powerupUI, RenderC), game->engine->GfxEngine, NullSprite);
    InitTextComponent(GetComponentData(powerupUI, TextC), "", Haettenschweiler, 18);
    battleLevelData->powerupUI = GetComponentData(powerupUI, TextC);
}