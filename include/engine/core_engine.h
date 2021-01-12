#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <stdlib.h>
#include "time.h"
#include "ecs.h"
#include "scenes_engine.h"
#include "graphics_engine.h"
#include "pools_engine.h"
#include "audio_engine.h"

//Main game Engine containing all the APIs needed to make the game work a game loop
struct Engine
{
    struct Time* time;  //struct containing time informations such as deltatime and deltatime in seconds
    struct GFXEngine* GfxEngine;    //the engine managing the screen draws and containing the renderer
    struct ScenesEngine* scenesEngine;    //the engine managing the scenes loads and changes
    struct PoolsEngine* poolsEngine;    //the engine managing the object pooling to save memory allocations at runtime
    struct AudioEngine* audioEngine;    //the engine managing musics and sound effects
    struct EntityComponentSystem* ECS;  //the engine managing the entities, their components, and their behaviours
};

//allocate and start the main engine and all the other engines
struct Engine* StartEngine();
//update the engines main stuff (like time)
void UpdateEngine(struct Engine* engine);
//stop and deallocate all the engines and the stuff related
void StopEngine(struct Engine* engine);
//reset all the needed engines and systems to start fresh. You should call this before loading a new scene at the end of the current frame.
void Reset(struct Engine* engine);
//get a random int between 0 and max
int GetRandomInt(int max);
//get a random int between min and max
int GetRandomIntBetween(int max, int min);
//get a random float between 0 and max
float GetRandomFloat(float max);
//get a random float between min and max
float GetRandomFloatBetween(float max, float min);

#endif //CORE_ENGINE_H