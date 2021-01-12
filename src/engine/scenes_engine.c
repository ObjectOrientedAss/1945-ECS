#include "scenes_engine.h"

struct ScenesEngine *__StartScenesEngine()
{
    struct ScenesEngine *scenesEngine = calloc(1, sizeof(struct ScenesEngine));
    scenesEngine->scenes = aiv_vector_new_with_cap(2);

    return scenesEngine;
}

void __DestroyScenesEngine(struct ScenesEngine *engine)
{
    for (uint i = 0; i < aiv_vector_size(engine->scenes); i++)
    {
        __DestroyScene(aiv_vector_at(engine->scenes, i));
    }

    aiv_vector_destroy(engine->scenes);
    free(engine);
}

void LoadScene(struct ScenesEngine *engine, struct Game *game, SceneType type)
{
    struct Scene *scene = aiv_vector_at(engine->scenes, (int)type);
    if (scene != NULL)
    {
        scene->Load(game);
    }
}

void CreateScene(struct ScenesEngine *engine, void (*Load)(struct Game *game), SceneType type)
{
    struct Scene *scene = calloc(1, sizeof(struct Scene));
    scene->type = type;
    scene->Load = Load;
    aiv_vector_add(engine->scenes, scene);
}

void __DestroyScene(struct Scene *scene)
{
    scene->Load = NULL;
    free(scene);
}