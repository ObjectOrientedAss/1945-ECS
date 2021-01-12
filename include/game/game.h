#ifndef GAME_H
#define GAME_H

#include "core_engine.h"

//basic game structure
struct Game
{
    void* levelData;        //this shouls contain the scene specific data, filled at scene loading, and used during the game loop
    float mousePositionX;   //X position of the mouse cursor on the screen
    float mousePositionY;   //Y position of the mouse cursor on the screen
    struct Engine* engine;  //the main engine
    boolean quitRequest;    //has the user requested to quit the game somehow during this frame?
    boolean mouseDown;      //is the left mouse button down?
    enum SceneType sceneToLoad;  //when you want to trigger a scene change (that will happen at the end of the current frame), put here the desired scene type and it will be loaded
};

//allocate the game and start the engines
struct Game* StartGame();
//deallocate and stop all the engines, free everything and quit the game
void QuitGame(struct Game* game);
//the main game loop frame update
void GameLoop(struct Game* game);
//poll the SDL events
boolean UpdateSDL(struct Game* game);
//update the ECS loop
void UpdateECS(struct Game* game);

#endif //GAME_H