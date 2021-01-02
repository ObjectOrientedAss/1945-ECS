#ifndef GAME_H
#define GAME_H

#include "scenes_engine.h"
#include "core_engine.h"
#include "ecs.h"
#include "behaviours.h"

struct Engine;
struct EntityComponentSystem;
struct Game
{
    boolean quitRequest;
    boolean mouseDown;
    float mousePositionX;
    float mousePositionY;
    SceneType sceneToLoad;             //when you want to trigger a scene change (that will happen at the end of the current frame), put here the desired enum value and it will be loaded
    struct Engine* engine;             //the main engine
    struct EntityComponentSystem* ECS; //the ecs system to manage all the entities, their components, and their behaviours in the current scene
};

struct Game* StartGame();
void QuitGame(struct Game* game);
void GameLoop(struct Game* game);
boolean UpdateSDL(struct Game* game);
void UpdateECS(struct Game* game);

#endif //GAME_H