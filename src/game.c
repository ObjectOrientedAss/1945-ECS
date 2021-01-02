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
        //AddComponent(entity, AnimatorC, AnimatorBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;
    case Enemy:
        AddComponent(entity, TransformC, NULL);
        AddComponent(entity, MovementC, EnemyMovementBehaviour);
        AddComponent(entity, PhysicsC, PhysicsBehaviour);
        AddComponent(entity, HealthC, HealthBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;
    case Button:
        AddComponent(entity, ButtonC, ButtonBehaviour);
        AddComponent(entity, RenderC, RenderBehaviour);
        break;
    }
}

void LoadMainMenu(struct Game *game)
{
    struct Entity *button = CreateEntity(Button, true, game->ECS);
    AddComponents(button, Button);
    InitButtonComponent(GetComponentData(button, ButtonC), StartGameClick, vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f));
    InitRenderComponent(GetComponentData(button, RenderC), game->engine->GfxEngine, StartButtonS);
    printf("MAIN MENU SCENE LOADED");
}

void LoadGameScene(struct Game* game)
{
    struct Entity *player = CreateEntity(Player, true, game->ECS);
    AddComponents(player, Player);
    InitTransformComponent(GetComponentData(player, TransformC), vec2_new(game->engine->GfxEngine->windowWidth * 0.5f, game->engine->GfxEngine->windowHeight * 0.5f));
    InitMovementComponent(GetComponentData(player, MovementC), vec2_new(0, 0), 25.0f);
    InitRenderComponent(GetComponentData(player, RenderC), game->engine->GfxEngine, NullS);
    //InitAnimatorComponent(GetComponentData(player, AnimatorC), game->engine->GfxEngine, PlayerA);
    InitPhysicsComponent(GetComponent(player, PhysicsC));
    printf("GAME SCENE LOADED");
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
    CreateScene(game->engine->scenesEngine, LoadMainMenu, MainMenuScene);
    CreateScene(game->engine->scenesEngine, LoadGameScene, GameScene);
    LoadScene(game->engine->scenesEngine, game, GameScene);
    game->sceneToLoad = None;

    while (!game->quitRequest)
    {
        UpdateEngine(game->engine);

        if (!UpdateSDL(game))
            break;

        UpdateECS(game);

        if (game->sceneToLoad != None)
        {
            game->ECS = ECSReset(game->ECS);
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

    SDL_SetRenderDrawColor(game->engine->GfxEngine->renderer, 255, 0, 0, 0);
    for (int i = 0; i < (int)Last; i++)
    {
        currentSystem = aiv_vector_at(game->ECS->systems, i);

        if (i == (int)Last - 1) //RENDERING SYSTEM, IS THE LAST ONE!
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