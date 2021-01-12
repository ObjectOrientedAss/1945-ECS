#include "game.h"

struct Game *StartGame()
{
    struct Game *game = (struct Game *)calloc(1, sizeof(struct Game));

    game->engine = StartEngine();
    if (game->engine == NULL)
        return NULL;

    return game;
}

void QuitGame(struct Game *game)
{
    StopEngine(game->engine);
    free(game);
}

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
    //prepare all the game scenes
    CreateScene(game->engine->scenesEngine, LoadSplashScreenScene, SplashScreenScene);
    CreateScene(game->engine->scenesEngine, LoadMainMenuScene, MainMenuScene);
    CreateScene(game->engine->scenesEngine, LoadGameScene, GameScene);
    //load the first scene of the game
    LoadScene(game->engine->scenesEngine, game, SplashScreenScene);
    game->sceneToLoad = None;

    //actual game loop
    while (!game->quitRequest)
    {
        UpdateEngine(game->engine); //update the main engine stuff

        if (!UpdateSDL(game)) //update sdl
            break;

        UpdateECS(game); //update the ECS

        if (game->sceneToLoad != None) //if in this frame has been requested a scene change
        {
            Reset(game->engine);
            free(game->levelData); //clear the scene data
            LoadScene(game->engine->scenesEngine, game, game->sceneToLoad); //and load the requested scene
            game->sceneToLoad = None;
        }
    }

    QuitGame(game); //shut down all the systems and close the game
}

void UpdateECS(struct Game *game)
{
    aiv_vector *currentSystem;
    struct Component *systemComponent;

    SDL_SetRenderDrawColor(game->engine->GfxEngine->renderer, 0, 0, 235, 0);
    for (int i = 0; i < (int)c_type_last; i++)
    {
        currentSystem = aiv_vector_at(game->engine->ECS->__systems, i);

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
    SDL_RenderPresent(game->engine->GfxEngine->renderer);
    Collect(game->engine->ECS);
}