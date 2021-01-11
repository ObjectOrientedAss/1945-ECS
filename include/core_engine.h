#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <stdlib.h>
#include "time.h"
#include "ecs.h"
#include "scenes_engine.h"
#include "graphics_engine.h"
#include "pools_engine.h"
#include "audio_engine.h"

struct Engine
{
    struct Time* time;
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