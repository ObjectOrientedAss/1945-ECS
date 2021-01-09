#include "game.h"

//DEFINE THE BASIC COMPONENTS FOR EACH ENTITY AND THEIR SYSTEM BEHAVIOUR
void AddComponents(struct Entity *entity)
{
    switch (entity->type)
    {
    case Player:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, InputC, InputBehaviour);
        AddComponent(entity, MovementC, PlayerMovementBehaviour);
        AddComponent(entity, PhysicsC, PhysicsBehaviour);
        AddComponent(entity, HealthC, HealthBehaviour);
        AddComponent(entity, AnimatorC, AnimatorBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        AddComponent(entity, ShootC, PlayerShootBehaviour);
        break;
    case Enemy:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, MovementC, AutomatedMovementBehaviour);
        AddComponent(entity, PhysicsC, PhysicsBehaviour);
        AddComponent(entity, HealthC, HealthBehaviour);
        AddComponent(entity, AnimatorC, AnimatorBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        AddComponent(entity, ShootC, EnemyShootBehaviour);
        break;
    case EnemyBullet:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, MovementC, AutomatedMovementBehaviour);
        AddComponent(entity, PhysicsC, PhysicsBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;
    case PlayerBullet:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, MovementC, AutomatedMovementBehaviour);
        AddComponent(entity, PhysicsC, PhysicsBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;
    case Button:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, UIC, UIBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;
    case Particle:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, TimedBehaviourC, ToggleEntityAfterBehaviour);
        AddComponent(entity, MovementC, AutomatedMovementBehaviour);
        AddComponent(entity, AnimatorC, AnimatorBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;
    case Background:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;
    case SceneManager:
        AddComponent(entity, TransformC, NULL);
        break;
    case AudioEmitter:
        AddComponent(entity, AudioC, NULL);
        AddComponent(entity, TimedBehaviourC, ToggleEntityAfterBehaviour);
        break;
    case Text:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, TextC, NULL);
        AddComponent(entity, RenderC, FontRenderBehaviour);
    }
}

void LoadMainMenu(struct Game *game)
{
    struct Entity *backGround = CreateEntity(Background, true, game->ECS);
    AddComponents(backGround);
    InitTransformComponent(GetComponentData(backGround, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f));
    InitRenderComponent(GetComponentData(backGround, RenderC), game->engine->GfxEngine, TitleS);

    struct Entity *buttonStart = CreateEntity(Button, true, game->ECS);
    AddComponents(buttonStart);
    InitTransformComponent(GetComponentData(buttonStart, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f + 75, game->engine->GfxEngine->windowHeight * 0.5f + 35));
    InitUIComponent(GetComponent(buttonStart, UIC), StartGameClick, ButtonHoverStart, ButtonHoverEnd);
    InitRenderComponent(GetComponentData(buttonStart, RenderC), game->engine->GfxEngine, StartButtonS);

    struct Entity *buttonQuit = CreateEntity(Button, true, game->ECS);
    AddComponents(buttonQuit);
    InitTransformComponent(GetComponentData(buttonQuit, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f + 75, game->engine->GfxEngine->windowHeight * 0.5f + 90));
    InitUIComponent(GetComponent(buttonQuit, UIC), QuitGameClick, ButtonHoverStart, ButtonHoverEnd);
    InitRenderComponent(GetComponentData(buttonQuit, RenderC), game->engine->GfxEngine, QuitButtonS);

    struct Entity *audioEmitter = CreateEntity(AudioEmitter, true, game->ECS);
    AddComponents(audioEmitter);
    InitAudioComponent(GetComponent(audioEmitter, AudioC), game->engine->audioEngine, BackgroundMusic, -1, SetAudio, PlayAudio);
    Mix_VolumeMusic(25);
    AudioComponent *audioComponent = GetComponentData(audioEmitter, AudioC);
    audioComponent->PlayAudio(GetComponent(audioEmitter, AudioC), game);

    printf("MAIN MENU SCENE LOADED");
}

void LoadGameScene(struct Game *game)
{
    struct BattleLevelData *battleLevelData = (struct BattleLevelData *)calloc(1, sizeof(struct BattleLevelData));

    battleLevelData->playerMaxLife = 100.0f;
    battleLevelData->playerBulletsDamage = 25.0f;
    battleLevelData->playerShootCooldown = 0.2f;
    battleLevelData->playerMaxLives = 1;
    battleLevelData->playerSpeed = 0.3f;

    battleLevelData->enemiesShootCooldown = 0.4f;
    battleLevelData->enemyBulletsDamage = 10.0f;
    battleLevelData->enemySpawnTimer = 1.4f;
    battleLevelData->enemySpawnTimerChanger = 0.4f;
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
    battleLevelData->squadronSpawnTimer = 8.0f;
    battleLevelData->enemyOndulationChance = 70;

    //CREATE AND INITIALIZE THE GAME SCENE MANAGER
    struct Entity *sceneManager = CreateEntity(SceneManager, true, game->ECS);
    AddComponents(sceneManager);
    InitTransformComponent(GetComponentData(sceneManager, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, -40));
    struct Component *c = NULL;
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnEnemyBehaviour);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->enemySpawnTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnIslandBehaviour);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->islandSpawnTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, ScoreDistanceBehaviour);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->increaseScoreTimer, NULL);
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnEnemySquadronBehaviour);
    InitTimedBehaviourComponent(c->data, -1, battleLevelData->squadronSpawnTimer, NULL);

    //CREATE AND INITIALIZE THE WATER BACKGROUNDS TO SWAP
    for (int i = 0; i < 2; i++)
    {
        struct Entity *waterBackground = CreateEntity(Background, true, game->ECS);
        AddComponents(waterBackground);
        AddComponent(waterBackground, MovementC, WaterMovementBehaviour);
        if (i == 0)
            InitTransformComponent(GetComponentData(waterBackground, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f));
        else
            InitTransformComponent(GetComponentData(waterBackground, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, -game->engine->GfxEngine->windowHeight * 0.5f));
        InitMovementComponent(GetComponentData(waterBackground, MovementC), vec2_new(0, 1), battleLevelData->environmentSpeed);
        InitRenderComponent(GetComponentData(waterBackground, RenderC), game->engine->GfxEngine, WaterS);
    }

    //CREATE AND INITIALIZE THE ISLANDS
    for (int i = 0; i < 15; i++)
    {
        printf("Creating islands...");
        struct Entity *island = CreateEntity(Background, false, game->ECS);
        AddComponents(island);
        AddComponent(island, MovementC, AutomatedMovementBehaviour);
        InitTransformComponent(GetComponentData(island, TransformC), vec2_new(-100, -100));
        InitMovementComponent(GetComponentData(island, MovementC), vec2_new(0, 1), battleLevelData->environmentSpeed);
        InitRenderComponent(GetComponentData(island, RenderC), game->engine->GfxEngine, Island1S);
        Enqueue(game->engine->poolsEngine, island);
    }

    //CREATE AND POOL THE PARTICLES
    for (int i = 0; i < 10; i++)
    {
        printf("Creating particles...");
        struct Entity *particle = CreateEntity(Particle, false, game->ECS);
        AddComponents(particle);
        InitTransformComponent(GetComponentData(particle, TransformC), vec2_new(-100, -100));
        InitTimedBehaviourComponent(GetComponentData(particle, TimedBehaviourC), 1, 0.9f, NULL);
        InitMovementComponent(GetComponentData(particle, MovementC), vec2_new(0, 0), 0);
        InitRenderComponent(GetComponentData(particle, RenderC), game->engine->GfxEngine, NullSprite);
        InitAnimatorComponent(GetComponent(particle, AnimatorC), game->engine->GfxEngine, EnemyExplosionA, 0.15f, SetAnimation);
        Enqueue(game->engine->poolsEngine, particle);
    }

    //CREATE AND POOL THE ENEMY BULLETS
    for (int i = 0; i < 60; i++)
    {
        printf("Creating enemy bullets...");
        struct Entity *enemyBullet = CreateEntity(EnemyBullet, false, game->ECS);
        AddComponents(enemyBullet);
        InitTransformComponent(GetComponentData(enemyBullet, TransformC), vec2_new(-100, -100));
        InitMovementComponent(GetComponentData(enemyBullet, MovementC), vec2_new(0, 1), battleLevelData->bulletSpeed);
        InitRenderComponent(GetComponentData(enemyBullet, RenderC), game->engine->GfxEngine, EnemyBullet1S);
        InitPhysicsComponent(GetComponent(enemyBullet, PhysicsC), Collide);
        Enqueue(game->engine->poolsEngine, enemyBullet);
    }

    //CREATE AND POOL THE PLAYER BULLETS
    for (int i = 0; i < 15; i++)
    {
        printf("Creating player bullets...");
        struct Entity *playerBullet = CreateEntity(PlayerBullet, false, game->ECS);
        AddComponents(playerBullet);
        InitTransformComponent(GetComponentData(playerBullet, TransformC), vec2_new(-200, -200));
        InitMovementComponent(GetComponentData(playerBullet, MovementC), vec2_new(0, -1), battleLevelData->bulletSpeed);
        InitRenderComponent(GetComponentData(playerBullet, RenderC), game->engine->GfxEngine, PlayerBulletS);
        InitPhysicsComponent(GetComponent(playerBullet, PhysicsC), Collide);
        Enqueue(game->engine->poolsEngine, playerBullet);
    }

    //CREATE AND POOL THE ENEMIES
    for (int i = 0; i < 20; i++)
    {
        printf("Creating enemies...");
        struct Entity *enemy = CreateEntity(Enemy, false, game->ECS);
        AddComponents(enemy);
        InitTransformComponent(GetComponentData(enemy, TransformC), vec2_new(-100, -100));
        InitHealthComponent(GetComponentData(enemy, HealthC), 1, 1, battleLevelData->enemyLife, battleLevelData->enemyLife, ChangeHealth, EnemyDeath);
        InitMovementComponent(GetComponentData(enemy, MovementC), vec2_new(0, 1), battleLevelData->enemySpeed);
        InitRenderComponent(GetComponentData(enemy, RenderC), game->engine->GfxEngine, NullSprite);
        InitAnimatorComponent(GetComponent(enemy, AnimatorC), game->engine->GfxEngine, Enemy1A, 0.3f, SetAnimation);
        InitPhysicsComponent(GetComponent(enemy, PhysicsC), Collide);
        InitShootComponent(GetComponentData(enemy, ShootC), battleLevelData->enemiesShootCooldown, EnemyBullet);
        Enqueue(game->engine->poolsEngine, enemy);
    }

    for (int i = 0; i < 20; i++)
    {
        printf("Creating audio emitters...");
        struct Entity *audioEmitter = CreateEntity(AudioEmitter, false, game->ECS);
        AddComponents(audioEmitter);
        InitAudioComponent(GetComponent(audioEmitter, AudioC), game->engine->audioEngine, BackgroundMusic, -1, SetAudio, PlayAudio);
        InitTimedBehaviourComponent(GetComponentData(audioEmitter, TimedBehaviourC), 1, 0.2f, NULL);
        Enqueue(game->engine->poolsEngine, audioEmitter);
    }

    //CREATE AND INITIALIZE THE PLAYER
    printf("Creating player...");
    struct Entity *player = CreateEntity(Player, true, game->ECS);
    AddComponents(player);
    InitTransformComponent(GetComponentData(player, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight - 100));
    InitHealthComponent(GetComponentData(player, HealthC), battleLevelData->playerMaxLives, battleLevelData->playerMaxLives, battleLevelData->playerMaxLife, battleLevelData->playerMaxLife, ChangeHealth, PlayerDeath);
    InitMovementComponent(GetComponentData(player, MovementC), vec2_new(0, 0), battleLevelData->playerSpeed);
    InitRenderComponent(GetComponentData(player, RenderC), game->engine->GfxEngine, NullSprite);
    InitAnimatorComponent(GetComponent(player, AnimatorC), game->engine->GfxEngine, PlayerA, 0.3f, SetAnimation);
    InitPhysicsComponent(GetComponent(player, PhysicsC), Collide);
    InitShootComponent(GetComponentData(player, ShootC), battleLevelData->playerShootCooldown, PlayerBullet);

    battleLevelData->player = player;

    struct Entity *bottomUI = CreateEntity(Background, true, game->ECS);
    AddComponents(bottomUI);
    InitTransformComponent(GetComponentData(bottomUI, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight - 38));
    InitRenderComponent(GetComponentData(bottomUI, RenderC), game->engine->GfxEngine, BottomS);
    printf("GAME SCENE LOADED");
    game->levelData = battleLevelData;

    int x = 35;
    int y = 425;
    battleLevelData->lives = aiv_vector_new_with_cap(battleLevelData->playerMaxLives);
    for (int i = 0; i < battleLevelData->playerMaxLives; i++)
    {
        struct Entity *playerLifeUI = CreateEntity(Background, true, game->ECS);
        AddComponents(playerLifeUI);
        InitTransformComponent(GetComponentData(playerLifeUI, TransformC), vec2_new(x, y));
        InitRenderComponent(GetComponentData(playerLifeUI, RenderC), game->engine->GfxEngine, LifeS);
        aiv_vector_add(battleLevelData->lives, playerLifeUI);
        x += 30;
    }

    struct Entity *scoreUI = CreateEntity(Text, true, game->ECS);
    AddComponents(scoreUI);
    InitTransformComponent(GetComponentData(scoreUI, TransformC), vec2_new(95, 455));
    InitRenderComponent(GetComponentData(scoreUI, RenderC), game->engine->GfxEngine, NullSprite);
    InitTextComponent(GetComponentData(scoreUI, TextC), "0", Haettenschweiler, 18);
    battleLevelData->scoreUI = GetComponentData(scoreUI, TextC);
}

int GetRandomInt(int max)
{
    return rand() % max;
}

int GetRandomIntBetween(int max, int min)
{
    return (rand() % (max + 1 - min)) + min;
}

float GetRandomFloat(float max)
{
    return (float)rand() / (float)(RAND_MAX / max);
}

float GetRandomFloatBetween(float max, float min)
{
    return (((float)rand() / (float)RAND_MAX) * (max - min)) + min;
}

struct Game *StartGame()
{
    struct Game *game = (struct Game *)calloc(1, sizeof(struct Game));

    game->engine = StartEngine();
    if (game->engine == NULL)
        return NULL;

    game->ECS = ECSInit();
    if (game->ECS == NULL)
        return NULL;

    srand(time(NULL));

    return game;
}

void QuitGame(struct Game *game)
{
    StopEngine(game->engine);
    DestroyECS(game->ECS);
    free(game);
}

//POLL SDL EVENTS TO DETECT CHANGES
boolean UpdateSDL(struct Game *game)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return false;
        case SDL_MOUSEBUTTONDOWN:
            game->mouseDown = true;
            break;
        case SDL_MOUSEBUTTONUP:
            game->mouseDown = false;
            break;
        case SDL_MOUSEMOTION:
            game->mousePositionX = event.motion.x;
            game->mousePositionY = event.motion.y;
            break;
        }
    }

    return true;
}

void GameLoop(struct Game *game)
{
    CreateScene(game->engine->scenesEngine, LoadMainMenu, MainMenuScene); //main menu missing audio for now...
    CreateScene(game->engine->scenesEngine, LoadGameScene, GameScene);
    LoadScene(game->engine->scenesEngine, game, MainMenuScene);
    game->sceneToLoad = None;

    while (!game->quitRequest)
    {
        // char title[100];
        // sprintf_s(title, sizeof(title), "Delta: time %f - Fps: %d", game->engine->time->deltaTime, game->engine->time->fps);
        // SDL_SetWindowTitle(game->engine->GfxEngine->window, title);
        UpdateEngine(game->engine);

        if (!UpdateSDL(game))
            break;

        UpdateECS(game);

        if (game->sceneToLoad != None)
        {
            game->ECS = ECSReset(game->ECS);
            game->engine->poolsEngine = PoolsReset(game->engine->poolsEngine);
            free(game->levelData);
            LoadScene(game->engine->scenesEngine, game, game->sceneToLoad);
            game->sceneToLoad = None;
        }
    }

    QuitGame(game);
}

void UpdateECS(struct Game *game)
{
    aiv_vector *currentSystem;
    struct Component *systemComponent;

    SDL_SetRenderDrawColor(game->engine->GfxEngine->renderer, 0, 0, 235, 0);
    for (int i = 0; i < (int)c_type_last; i++)
    {
        currentSystem = aiv_vector_at(game->ECS->systems, i);

        if (i == (int)c_type_last - 1) //RENDERING SYSTEM, IS THE LAST ONE!
            SDL_RenderClear(game->engine->GfxEngine->renderer);

        for (uint j = 0; j < aiv_vector_size(currentSystem); j++)
        {
            systemComponent = aiv_vector_at(currentSystem, j);

            if (systemComponent->owner->active)
            {
                if (systemComponent->active)
                    systemComponent->behaviour(systemComponent, game);
            }
        }
    }
    //printf("Ancora non me so spaccato");
    SDL_RenderPresent(game->engine->GfxEngine->renderer);
    DestroyComponents(game->ECS->componentsToDestroy);
    DestroyEntities(game->ECS->entitiesToDestroy);
}