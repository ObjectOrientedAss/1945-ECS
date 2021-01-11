#ifndef GAME_H
#define GAME_H

#include "core_engine.h"

struct Game
{
    void* levelData;
    float mousePositionX;
    float mousePositionY;
    struct Engine* engine;             //the main engine
    struct EntityComponentSystem* ECS; //the ecs system to manage all the entities, their components, and their behaviours in the current scene
    boolean quitRequest;
    boolean mouseDown;
    enum SceneType sceneToLoad;             //when you want to trigger a scene change (that will happen at the end of the current frame), put here the desired enum value and it will be loaded
};

struct Game* StartGame();
void QuitGame(struct Game* game);
void GameLoop(struct Game* game);
boolean UpdateSDL(struct Game* game);
void UpdateECS(struct Game* game);
int GetRandomInt(int max);
int GetRandomIntBetween(int max, int min);
float GetRandomFloat(float max);
float GetRandomFloatBetween(float max, float min);

#endif //GAME_H