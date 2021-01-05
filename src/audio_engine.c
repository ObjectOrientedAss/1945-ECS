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

    //load chunks
    LoadChunks(engine);
    //load musics
    LoadMusics(engine);
    return engine;
}

void StopAudioEngine(struct AudioEngine* engine, AudioType type)
{
    for (int i = aiv_vector_size(engine->chunks) - 1; i >= 0; i--)
    {
        Mix_Chunk* c = aiv_vector_remove_at(engine->chunks, i);
        Mix_FreeChunk(c);    
    }
    aiv_vector_destroy(engine->chunks);
    
    for (int i = aiv_vector_size(engine->musics) - 1; i >= 0; i--)
    {
        Mix_Music* m = aiv_vector_remove_at(engine->musics, i);
        Mix_FreeMusic(m);    
    }
    aiv_vector_destroy(engine->musics);
    free(engine);
}

void LoadChunks(struct AudioEngine *engine)
{

}

void LoadMusics(struct AudioEngine *engine)
{

}

void LoadSound(struct AudioEngine *engine, AudioType audioType)
{

}