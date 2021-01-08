#ifndef SCENES_ENGINE_H
#define SCENES_ENGINE_H

#include "aiv-vector.h"

enum t_scene { None = -1, MainMenuScene, GameScene };
typedef enum t_scene SceneType;

struct ScenesEngine
{
    aiv_vector* scenes;
};

struct SceneData
{
    void* data;
    void (*InitializeGameData)(struct GameSceneData* gameSceneData);
};

struct GameSceneData
{
    struct Entity* player;
    aiv_vector* lives;
    double enemySpawnTimer;
    double islandSpawnTimer;
};

struct Game;
struct Scene
{
    struct SceneData* sceneData;
    void(*Load)(struct Game* game);
    SceneType type;
};

struct ScenesEngine* StartScenesEngine();
void DestroyScenesEngine(struct ScenesEngine* engine);
void LoadScene(struct ScenesEngine* engine, struct Game* game, SceneType type);
void CreateScene(struct ScenesEngine* engine, void(*Load)(struct Game* game), SceneType type);
void UpdateScene(struct Scene* scene);
void DestroyScene(struct Scene* scene);

#endif //SCENES_ENGINE_H