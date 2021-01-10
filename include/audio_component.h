#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H
#include "ecs.h"

struct AudioComponent
{
    struct AudioEngine* engine;
    struct Audio audio;
    int loops;
    void (*SetAudio)(struct Component* selfComponent, AudioType type, int loops);
    void (*PlayAudio)(struct Component *selfComponent, struct Game *game);
    boolean isPlaying;
} AudioComponent;

//INIT
void InitAudioComponent(struct Component *selfComponent, struct AudioEngine *engine, AudioType audioType, int loops, void (*SetAudio)(struct Component* selfComponent, AudioType type, int loops), void (*PlayAudio)(struct Component *selfComponent, struct Game *game));

//CUSTOM
void PlayAudio(struct Component *selfComponent, struct Game *game);
void SetAudio(struct Component* selfComponent, AudioType type, int loops);

#endif //AUDIO_COMPONENT_H