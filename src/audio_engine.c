#include "audio_engine.h"

struct AudioEngine *StartAudioEngine()
{
    struct AudioEngine *engine = (struct AudioEngine *)calloc(1, sizeof(struct AudioEngine));

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf("SDL Audio could not initialize! SDL Audio Error: %s\n", SDL_GetError());
        return NULL;
    }

    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640))
    {
        printf("SDL Mix Open Audio could not initialize! SDL Audio Error: %s\n", SDL_GetError());
        return NULL;
    }

    engine->sounds = aiv_vector_new_with_cap(3);

    //load sounds
    LoadSounds(engine);
    return engine;
}

void StopAudioEngine(struct AudioEngine *engine)
{
    for (int i = aiv_vector_size(engine->sounds) - 1; i >= 0; i--)
    {
        struct Audio *audio = aiv_vector_remove_at(engine->sounds, i);
        switch (audio->audioExtension)
        {
        case WAV:
            Mix_FreeChunk(audio->data);
            break;
        case MP3:
            Mix_FreeMusic(audio->data);
            break;
        }
        free(audio);
    }
    aiv_vector_destroy(engine->sounds);
    
    free(engine);
}

void LoadSounds(struct AudioEngine *engine)
{
    struct Audio *sound = NULL;
    sound = LoadSound("resources/assets/audio/background.mp3", MP3);
    aiv_vector_add(engine->sounds, sound);
    sound = LoadSound("resources/assets/audio/explosion_01.mp3", MP3);
    aiv_vector_add(engine->sounds, sound);
    sound = LoadSound("resources/assets/audio/explosion_02.mp3", MP3);
    aiv_vector_add(engine->sounds, sound);
}

struct Audio *LoadSound(char *path, AudioExtension audioExtension)
{
    struct Audio *sound = (struct Audio *)calloc(1, sizeof(struct Audio));
    sound->audioExtension = audioExtension;
    switch (sound->audioExtension)
    {
    case WAV:
        sound->data = (Mix_Chunk *)Mix_LoadWAV(path);
        break;
    case MP3:
        sound->data = (Mix_Music *)Mix_LoadMUS(path);
        break;
    }

    return sound;
}

struct Audio GetSound(struct AudioEngine *engine, AudioType type)
{
    struct Audio *sound = aiv_vector_at(engine->sounds, (int)type);
    struct Audio s = *sound;
    return s;
}