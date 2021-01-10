#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <stdlib.h>
#include "time.h"
#include "graphics_engine.h"
#include "scenes_engine.h"
#include "pools_engine.h"
#include "audio_engine.h"
#include "ecs.h"

struct Engine
{
    Time* time;
    struct GFXEngine* GfxEngine;
    struct ScenesEngine* scenesEngine;
    struct PoolsEngine* poolsEngine;
    struct AudioEngine* audioEngine;
    struct Scene* currentScene;
};

struct Engine* StartEngine();
void UpdateEngine(struct Engine* engine);
void StopEngine(struct Engine* engine);

#endif //CORE_ENGINE_H