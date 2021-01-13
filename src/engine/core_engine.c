#include "core_engine.h"

struct Engine *CoreEngineInit()
{
    struct Engine *engine = calloc(1, sizeof(struct Engine));

    engine->scenesEngine = __ScenesEngineInit();
    if (engine->scenesEngine == NULL)
        return NULL;

    engine->GfxEngine = __GFXEngineInit();
    if (engine->GfxEngine == NULL)
        return NULL;

    engine->poolsEngine = __PoolsEngineInit();
    if (engine->poolsEngine == NULL)
        return NULL;

    engine->audioEngine = __AudioEngineInit();
    if (engine->audioEngine == NULL)
        return NULL;

    engine->time = __TimeInit();
    if (engine->time == NULL)
        return NULL;

    engine->ECS = __ECSInit();
    if (engine->ECS == NULL)
        return NULL;

    srand(time(NULL));

    return engine;
}

void UpdateEngine(struct Engine *engine)
{
    __TimeUpdate(engine->time);
}

void StopEngine(struct Engine *engine)
{
    free(engine->time);
    __StopGFXEngine(engine->GfxEngine);
    __StopAudioEngine(engine->audioEngine);
    __DestroyScenesEngine(engine->scenesEngine);
    __DestroyPoolsEngine(engine->poolsEngine);
    __DestroyECS(engine->ECS);
    free(engine);
    SDL_Quit();
}

void Reset(struct Engine *engine)
{
    engine->ECS = __ECSReset(engine->ECS);
    engine->poolsEngine = __PoolsReset(engine->poolsEngine);
}

int GetRandomInt(int max)
{
    return rand() % max;
}

int GetRandomIntBetween(int max, int min)
{
    return (rand() % (max + 1 - min)) + min;
}

float GetRandomFloat(float max)
{
    return (float)rand() / (float)(RAND_MAX / max);
}

float GetRandomFloatBetween(float max, float min)
{
    return (((float)rand() / (float)RAND_MAX) * (max - min)) + min;
}