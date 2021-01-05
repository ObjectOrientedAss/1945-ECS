#include "game.h"

//DEFINE THE COMPONENTS AND THEIR BEHAVIOUR FOR EACH ENTITY HERE
void AddComponents(struct Entity *entity, EntityType entityType)
{
    switch (entityType)
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
        AddComponent(entity, ButtonC, ButtonBehaviour);
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
    case SceneManager:
        AddComponent(entity, TransformC, NULL);
        break;
    }
}

void LoadMainMenu(struct Game *game)
{
    struct Entity *backGround = CreateEntity(Background, true, game->ECS);
    AddComponents(backGround, Background);
    InitTransformComponent(GetComponentData(backGround, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f));
    InitRenderComponent(GetComponentData(backGround, RenderC), game->engine->GfxEngine, TitleS);

    struct Entity *buttonStart = CreateEntity(Button, true, game->ECS);
    AddComponents(buttonStart, Button);
    InitTransformComponent(GetComponentData(buttonStart, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f + 75, game->engine->GfxEngine->windowHeight * 0.5f + 35));
    InitButtonComponent(GetComponent(buttonStart, ButtonC), StartGameClick, ButtonHoverStart, ButtonHoverEnd);
    InitRenderComponent(GetComponentData(buttonStart, RenderC), game->engine->GfxEngine, StartButtonS);

    struct Entity *buttonQuit = CreateEntity(Button, true, game->ECS);
    AddComponents(buttonQuit, Button);
    InitTransformComponent(GetComponentData(buttonQuit, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f + 75, game->engine->GfxEngine->windowHeight * 0.5f + 90));
    InitButtonComponent(GetComponent(buttonQuit, ButtonC), QuitGameClick, ButtonHoverStart, ButtonHoverEnd);
    InitRenderComponent(GetComponentData(buttonQuit, RenderC), game->engine->GfxEngine, QuitButtonS);

    printf("MAIN MENU SCENE LOADED");
}

void LoadGameScene(struct Game *game)
{
    //CREATE AND INITIALIZE THE GAME SCENE MANAGER
    struct Entity *sceneManager = CreateEntity(SceneManager, true, game->ECS);
    AddComponents(sceneManager, SceneManager);
    InitTransformComponent(GetComponentData(sceneManager, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, -40));
    struct Component *c = NULL;
    c = AddComponent(sceneManager, TimedBehaviourC, SpawnEnemyBehaviour);
    InitTimedBehaviourComponent(c->data, -1, 3.0f);
    // c = AddComponent(sceneManager, TimedBehaviourC, SpawnIslandBehaviour);
    // InitTimedBehaviourComponent(c->data, -1, 2.5f);

    //CREATE AND INITIALIZE THE WATER BACKGROUNDS TO SWAP
    for (int i = 0; i < 2; i++)
    {
        struct Entity *waterBackground = CreateEntity(Background, true, game->ECS);
        AddComponents(waterBackground, Background);
        AddComponent(waterBackground, MovementC, WaterMovementBehaviour);
        if (i == 0)
            InitTransformComponent(GetComponentData(waterBackground, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f));
        else
            InitTransformComponent(GetComponentData(waterBackground, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, -game->engine->GfxEngine->windowHeight * 0.5f));
        InitMovementComponent(GetComponentData(waterBackground, MovementC), vec2_new(0, 1), 0.1f);
        InitRenderComponent(GetComponentData(waterBackground, RenderC), game->engine->GfxEngine, WaterS);
    }

    //CREATE AND POOL THE PARTICLES
    for (int i = 0; i < 10; i++)
    {
        printf("Creating particles...");
        struct Entity *particle = CreateEntity(Particle, false, game->ECS);
        AddComponents(particle, Particle);
        InitTransformComponent(GetComponentData(particle, TransformC), vec2_new(-100, -100));
        InitTimedBehaviourComponent(GetComponentData(particle, TimedBehaviourC), 1, 0.9f);
        InitMovementComponent(GetComponentData(particle, MovementC), vec2_new(0, 0), 0);
        InitRenderComponent(GetComponentData(particle, RenderC), game->engine->GfxEngine, NullS);
        InitAnimatorComponent(GetComponent(particle, AnimatorC), game->engine->GfxEngine, EnemyExplosionA, 0.15f, SetAnimation);
        Enqueue(game->engine->poolsEngine, particle);
    }

    //CREATE AND POOL THE ENEMY BULLETS
    for (int i = 0; i < 50; i++)
    {
        printf("Creating enemy bullets...");
        struct Entity *enemyBullet = CreateEntity(EnemyBullet, false, game->ECS);
        AddComponents(enemyBullet, EnemyBullet);
        InitTransformComponent(GetComponentData(enemyBullet, TransformC), vec2_new(-100, -100));
        InitMovementComponent(GetComponentData(enemyBullet, MovementC), vec2_new(0, 1), 0.2f);
        InitRenderComponent(GetComponentData(enemyBullet, RenderC), game->engine->GfxEngine, EnemyBullet1S);
        InitPhysicsComponent(GetComponent(enemyBullet, PhysicsC), Collide);
        Enqueue(game->engine->poolsEngine, enemyBullet);
    }

    //CREATE AND POOL THE PLAYER BULLETS
    for (int i = 0; i < 50; i++)
    {
        printf("Creating player bullets...");
        struct Entity *playerBullet = CreateEntity(PlayerBullet, false, game->ECS);
        AddComponents(playerBullet, PlayerBullet);
        InitTransformComponent(GetComponentData(playerBullet, TransformC), vec2_new(-200, -200));
        InitMovementComponent(GetComponentData(playerBullet, MovementC), vec2_new(0, -1), 0.2f);
        InitRenderComponent(GetComponentData(playerBullet, RenderC), game->engine->GfxEngine, PlayerBulletS);
        InitPhysicsComponent(GetComponent(playerBullet, PhysicsC), Collide);
        Enqueue(game->engine->poolsEngine, playerBullet);
    }

    //CREATE AND POOL THE ENEMIES
    for (int i = 0; i < 20; i++)
    {
        printf("Creating enemies...");
        struct Entity *enemy = CreateEntity(Enemy, false, game->ECS);
        AddComponents(enemy, Enemy);
        InitTransformComponent(GetComponentData(enemy, TransformC), vec2_new(-100, -100));
        InitHealthComponent(GetComponentData(enemy, HealthC), 1, 1, 100.0f, 100.0f, ChangeHealth, EnemyDeath);
        InitMovementComponent(GetComponentData(enemy, MovementC), vec2_new(0, 1), 0.05f);
        InitRenderComponent(GetComponentData(enemy, RenderC), game->engine->GfxEngine, NullS);
        InitAnimatorComponent(GetComponent(enemy, AnimatorC), game->engine->GfxEngine, Enemy1A, 0.3f, SetAnimation);
        InitPhysicsComponent(GetComponent(enemy, PhysicsC), Collide);
        InitShootComponent(GetComponentData(enemy, ShootC), 2.0, EnemyBullet);
        Enqueue(game->engine->poolsEngine, enemy);
    }

    //CREATE AND INITIALIZE THE PLAYER
    printf("Creating player...");
    struct Entity *player = CreateEntity(Player, true, game->ECS);
    AddComponents(player, Player);
    InitTransformComponent(GetComponentData(player, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight - 100));
    InitHealthComponent(GetComponentData(player, HealthC), 4, 3, 100.0f, 100.0f, ChangeHealth, PlayerDeath);
    InitMovementComponent(GetComponentData(player, MovementC), vec2_new(0, 0), 0.2f);
    InitRenderComponent(GetComponentData(player, RenderC), game->engine->GfxEngine, NullS);
    InitAnimatorComponent(GetComponent(player, AnimatorC), game->engine->GfxEngine, PlayerA, 0.3f, SetAnimation);
    InitPhysicsComponent(GetComponent(player, PhysicsC), Collide);
    InitShootComponent(GetComponentData(player, ShootC), 1.0, PlayerBullet);

    // struct Entity *enemy1 = Dequeue(game->engine->poolsEngine, Enemy);
    // InitTransformComponent(GetComponentData(enemy1, TransformC), vec2_new(100, 200));
    // SetEntityActiveStatus(enemy1, true);
    // Enqueue(game->engine->poolsEngine, enemy1);

    // struct Entity *enemy2 = Dequeue(game->engine->poolsEngine, Enemy);
    // InitTransformComponent(GetComponentData(enemy2, TransformC), vec2_new(300, 200));
    // SetEntityActiveStatus(enemy2, true);
    // Enqueue(game->engine->poolsEngine, enemy2);
    // printf("GAME SCENE LOADED");
}

int GetRandomInt(int max)
{
    return rand() % max;
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
    LoadScene(game->engine->scenesEngine, game, GameScene);
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