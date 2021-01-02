#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include "graphics_engine.h"
#include "time.h"
#include "scenes_engine.h"

struct Engine
{
    Time* time;
    struct GFXEngine* GfxEngine;
    struct ScenesEngine* scenesEngine;
    struct Scene* currentScene;
};

struct Engine* StartEngine();
void UpdateEngine(struct Engine* engine);
void StopEngine(struct Engine* engine);

#endif //CORE_ENGINE_H