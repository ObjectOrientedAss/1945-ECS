#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "SDL.h"
#include "SDL_mixer.h"
#include "common.h"
#include "aiv-vector.h"

//wav sounds are used as chunks, mp3 sounds are used as music
enum audio_extension
{
    WAV,
    MP3,
};
typedef enum audio_extension AudioExtension;

//the enum containing all the available sounds in the game
enum t_audio
{
    BackgroundMusic,
    Explosion1SFX,
    Explosion2SFX
};
typedef enum t_audio AudioType;

//the basic audio struct
struct Audio
{
    void* data;                     //chunk or music pointer
    AudioExtension audioExtension;  //the audio extension, MP3 or WAV
};

//the audio engine, it handles the audio stuff in the game and holds all the available sounds loaded
struct AudioEngine
{
    aiv_vector* sounds;            //the vector containing all the sound effects
};

//allocate and start the audio engine
struct AudioEngine* __AudioEngineInit();
//stop and deallocate the audio engine
void __StopAudioEngine(struct AudioEngine* engine);
//load all the game sounds to the audio engine
void __LoadSounds(struct AudioEngine* engine);
//load a single sound into the audio engine
struct Audio *__LoadSound(char *path, AudioExtension audioExtension);
//get a sound to be played from the audio engine
struct Audio GetSound(struct AudioEngine *engine, AudioType type);

#endif //AUDIO_ENGINE_H