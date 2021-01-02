#include "core_engine.h"

struct Engine *StartEngine()
{
    struct Engine *engine = calloc(1, sizeof(struct Engine));

    engine->scenesEngine = StartScenesEngine();
    if (engine->scenesEngine == NULL)
        return NULL;

    engine->GfxEngine = StartGFXEngine();
    if (engine->GfxEngine == NULL)
        return NULL;

    engine->time = TimeInit();
    if (engine->time == NULL)
        return NULL;

    //printf("Engine initialized correctly");

    return engine;
}

void UpdateEngine(struct Engine *engine)
{
    TimeUpdate(engine->time);
}

void StopEngine(struct Engine *engine)
{
    free(engine->time);
    StopGFXEngine(engine->GfxEngine);
    DestroyScenesEngine(engine->scenesEngine);
    SDL_Quit();
}