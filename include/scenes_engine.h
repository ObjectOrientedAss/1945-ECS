#ifndef SCENES_ENGINE_H
#define SCENES_ENGINE_H

#include "aiv-vector.h"

enum t_scene { None = -1, MainMenuScene, GameScene };
typedef enum t_scene SceneType;

struct ScenesEngine
{
    aiv_vector* scenes;
};

struct Game;
struct Scene
{
    SceneType type;
    void(*Load)(struct Game* game);
};

struct ScenesEngine* StartScenesEngine();
void DestroyScenesEngine(struct ScenesEngine* engine);
void LoadScene(struct ScenesEngine* engine, struct Game* game, SceneType type);
void CreateScene(struct ScenesEngine* engine, void(*Load)(struct Game* game), SceneType type);
void UpdateScene(struct Scene* scene);
void DestroyScene(struct Scene* scene);

#endif //SCENES_ENGINE_H