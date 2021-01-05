#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "SDL.h"
#include "SDL_mixer.h"
#include "common.h"
#include "aiv-vector.h"

enum audio_extension
{
    WAV,
    MP3,
};
typedef enum audio_extension AudioExtension;

enum t_audio
{
    Music,
    Explosion1,
    Explosion2
};
typedef enum t_audio AudioType;

struct AudioEngine
{
    aiv_vector* chunks;
    aiv_vector* musics;
};

struct AudioEngine* StartAudioEngine();
void StopAudioEngine();
void LoadChunks(struct AudioEngine* engine);
void LoadMusics(struct AudioEngine* engine);
void LoadSound(struct AudioEngine* engine, AudioType audioType);

#endif //AUDIO_ENGINE_H