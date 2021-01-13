#ifndef SCENES_ENGINE_H
#define SCENES_ENGINE_H

#include "scene.h"

//the list of scenes in the game
enum t_scene
{
    None = -1,
    SplashScreenScene,
    MainMenuScene,
    GameScene
};
typedef enum t_scene SceneType;

//basic scene structure
struct Scene
{
    void (*Load)(struct Game *game);    //the function called when the scene is loaded, it should setup and init your level
    SceneType type;                     //the scene type to easily find it in the scenes list
};

//the engine containing and managing all the game scenes
struct ScenesEngine
{
    aiv_vector *scenes;
};

//allocate and start the scenes engine
struct ScenesEngine *__ScenesEngineInit();
//deallocate and stop all the scenes and the scenes engine
void __DestroyScenesEngine(struct ScenesEngine *engine);
//load the scene of the requested type
void LoadScene(struct ScenesEngine *engine, struct Game *game, SceneType type);
//create a scene with the given informations
void CreateScene(struct ScenesEngine *engine, void (*Load)(struct Game *game), SceneType type);
//destroy and deallocate a scene
void __DestroyScene(struct Scene *scene);

#endif //SCENES_ENGINE_H