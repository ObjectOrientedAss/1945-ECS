#include "audio_component.h"

//set the audio on an audio component (it will play when the entity is active)
void SetAudio(struct Component *selfComponent, AudioType type, int loops)
{
    struct AudioComponent *audioComponent = selfComponent->data;
    audioComponent->audio = GetSound(audioComponent->engine, type);
    audioComponent->loops = loops;
}

void PlayAudio(struct Component *selfComponent, struct Game *game)
{
    struct AudioComponent *audioComponent = selfComponent->data;
    switch (audioComponent->audio.audioExtension)
    {
    case WAV:
        Mix_PlayChannel(-1, audioComponent->audio.data, audioComponent->loops);
        break;
    case MP3:
        Mix_PlayMusic(audioComponent->audio.data, audioComponent->loops);
        break;
    }
}

void InitAudioComponent(struct Component *selfComponent, struct AudioEngine *engine, AudioType audioType, int loops, void (*SetAudio)(struct Component *selfComponent, AudioType type, int loops), void (*PlayAudio)(struct Component *selfComponent, struct Game *game))
{
    struct AudioComponent *ac = selfComponent->data;
    ac->engine = engine;
    ac->SetAudio = SetAudio;
    ac->SetAudio(selfComponent, audioType, loops);
    ac->PlayAudio = PlayAudio;
}