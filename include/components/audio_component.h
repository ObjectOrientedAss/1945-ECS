#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H
#include "ecs.h"

//AUDIO DATA COMPONENT
struct AudioComponent
{
    struct AudioEngine* engine; //the audio engine from which to play sounds
    struct Audio audio;         //the audio structure containing the audio informations and the actual sound
    int loops;  //how many times should this audio play
    void (*SetAudio)(struct Component* selfComponent, AudioType type, int loops); //function called whenever you want to set a new audio
    void (*PlayAudio)(struct Component *selfComponent, struct Game *game);  //function called whenever you want to play the audio of this component
} AudioComponent;

//INIT
void InitAudioComponent(struct Component *selfComponent, struct AudioEngine *engine, AudioType audioType, int loops, void (*SetAudio)(struct Component* selfComponent, AudioType type, int loops), void (*PlayAudio)(struct Component *selfComponent, struct Game *game));

//CUSTOM
void PlayAudio(struct Component *selfComponent, struct Game *game);
void SetAudio(struct Component* selfComponent, AudioType type, int loops);

#endif //AUDIO_COMPONENT_H