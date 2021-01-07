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
    BackgroundMusic,
    Explosion1SFX,
    Explosion2SFX
};
typedef enum t_audio AudioType;

struct Audio
{
    void* data;                     //chunk or music pointer
    AudioExtension audioExtension;  //the audio extension, MP3 or WAV
};

struct AudioEngine
{
    aiv_vector* sounds;            //the vector containing all the sound effects
};

struct AudioEngine* StartAudioEngine();
void StopAudioEngine(struct AudioEngine* engine);
void LoadSounds(struct AudioEngine* engine);
struct Audio *LoadSound(char *path, AudioExtension audioExtension);
struct Audio GetSound(struct AudioEngine *engine, AudioType type);

#endif //AUDIO_ENGINE_H