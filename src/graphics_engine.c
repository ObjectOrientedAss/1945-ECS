#include "graphics_engine.h"

struct GFXEngine *StartGFXEngine()
{
    struct GFXEngine *engine = calloc(1, sizeof(struct GFXEngine));
    SDL_Init(SDL_INIT_VIDEO);

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int sdl_init_flags = IMG_Init(flags);

    if ((sdl_init_flags & flags) != flags)
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return NULL;
    }

    engine->window = SDL_CreateWindow("1945", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    if (engine->window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window Creation FAILED: %s", SDL_GetError());
        return NULL;
    }

    int x;
    int y;
    SDL_GetWindowSize(engine->window, &x, &y);
    engine->windowWidth = x;
    engine->windowHeight = y;
    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED);

    if (engine->renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create renderer: %s", SDL_GetError());
        return NULL;
    }

    engine->spriteSheets = aiv_vector_new();
    LoadSpriteSheets(engine);
    engine->sprites = aiv_vector_new();
    LoadSprites(engine);

    if (TTF_Init() == -1)
    {
        printf("SDL_TTF Init Failed.");
        return NULL;
    }
    //create the font paths and store them:
    engine->fonts = aiv_vector_new_with_cap(1);
    CreateFonts(engine);

    return engine;
}

void StopGFXEngine(struct GFXEngine *engine)
{
    TTF_Quit();

    //clear the spritesheets lists
    for (uint i = 0; i < aiv_vector_size(engine->spriteSheets); i++)
    {
        struct SpriteSheet *s = aiv_vector_at(engine->spriteSheets, i);
        SDL_DestroyTexture(s->texture);
        free(s);
    }
    aiv_vector_destroy(engine->spriteSheets);

    //clear the sprites list
    for (uint i = 0; i < aiv_vector_size(engine->sprites); i++)
    {
        struct Sprite *s = aiv_vector_at(engine->sprites, i);
        SDL_DestroyTexture(s->texture);
        free(s);
    }
    aiv_vector_destroy(engine->sprites);

    for (uint i = 0; i < aiv_vector_size(engine->fonts); i++)
    {
        FC_FreeFont(aiv_vector_at(engine->fonts, i));
    }
    aiv_vector_destroy(engine->fonts);

    //quit SDL gfx
    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);
    free(engine);
}

struct Sprite *CreateSprite(char *path, SpriteType type, SDL_Renderer *renderer)
{
    struct Sprite *sprite = calloc(1, sizeof(struct Sprite));
    SDL_Rect originRect;
    SDL_Rect spriteRect;
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    sprite->texture = texture;
    int h = 0;
    int w = 0;
    SDL_QueryTexture(sprite->texture, NULL, NULL, &h, &w);
    //printf("%d - %d\n", h, w);

    originRect.w = h;
    originRect.h = w;
    originRect.x = 0;
    originRect.y = 0;

    //printf("Creating SpriteType: %d | Texture W: %d | Texture H: %d\n", (int)type, sprite->originRect->w, sprite->originRect->h);
    spriteRect.w = originRect.w;
    spriteRect.h = originRect.h;
    spriteRect.x = 0;
    spriteRect.y = 0;

    sprite->originRect = originRect;
    sprite->spriteRect = spriteRect;
    // printf("\n\nOR H: %d | OR W: %d | OR X: %d | OR Y: %d", sprite->originRect.h, sprite->originRect.w, sprite->originRect.x, sprite->originRect.y);
    // printf("\n\nSR H: %d | SR W: %d | SR X: %d | SR Y: %d", sprite->spriteRect.h, sprite->spriteRect.w, sprite->spriteRect.x, sprite->spriteRect.y);
    // printf("\n\nSPRITE TEXTURE %llu", sprite->texture);

    return sprite;
}

void LoadSprites(struct GFXEngine *engine)
{
    struct Sprite *sprite = NULL;
    //and now my big mascotte fella...
    sprite = CreateSprite("resources/assets/NotFound.png", NullSprite, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/StartButton.png", StartButtonS, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/QuitButton.png", QuitButtonS, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/enemy/enemybullet1.png", EnemyBullet1S, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/enemy/enemybullet2.png", EnemyBullet2S, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/player/bullet.png", PlayerBulletS, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/ui/Title.png", TitleS, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/map/water-bg.png", WaterS, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/map/island1.png", Island1S, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/map/island2.png", Island2S, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/map/island3.png", Island3S, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/ui/bottom.png", BottomS, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/ui/life.png", LifeS, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/player/attack_powerup.png", AttackPowerupS, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/player/speed_powerup.png", SpeedPowerupS, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
    sprite = CreateSprite("resources/assets/player/life_powerup.png", LifePowerupS, engine->renderer);
    aiv_vector_add(engine->sprites, sprite);
}

struct SpriteSheet *CreateSpriteSheet(char *path, int frames, float baseFrameDuration, AnimationType type, SDL_Renderer *renderer)
{
    struct SpriteSheet *spriteSheet = calloc(1, sizeof(struct SpriteSheet));
    spriteSheet->frames = frames;
    spriteSheet->type = type;
    spriteSheet->baseFrameDuration = baseFrameDuration;
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    spriteSheet->texture = texture;
    int spriteSheetW;
    SDL_QueryTexture(spriteSheet->texture, NULL, NULL, &spriteSheetW, &spriteSheet->frameHeight);
    spriteSheet->frameWidth = spriteSheetW / spriteSheet->frames;
    printf("Frames: %d, frameWidth: %d", spriteSheet->frames, spriteSheet->frameWidth);
    return spriteSheet;
}

void LoadSpriteSheets(struct GFXEngine *engine)
{
    //CREATE THE SPRITESHEETS
    struct SpriteSheet *spriteSheet = NULL;

    spriteSheet = CreateSpriteSheet("resources/assets/NotFound.png", 1, 0.1f, NullAnimation, engine->renderer);
    aiv_vector_add(engine->spriteSheets, spriteSheet);
    spriteSheet = CreateSpriteSheet("resources/assets/enemy/enemy1_strip3.png", 3, 0.3f, Enemy1A, engine->renderer);
    aiv_vector_add(engine->spriteSheets, spriteSheet);
    spriteSheet = CreateSpriteSheet("resources/assets/enemy/enemy2_strip3.png", 3, 0.3f, Enemy2A, engine->renderer);
    aiv_vector_add(engine->spriteSheets, spriteSheet);
    spriteSheet = CreateSpriteSheet("resources/assets/enemy/enemy3_strip3.png", 3, 0.3f, Enemy3A, engine->renderer);
    aiv_vector_add(engine->spriteSheets, spriteSheet);
    spriteSheet = CreateSpriteSheet("resources/assets/enemy/enemy4_strip3.png", 3, 0.3f, Enemy4A, engine->renderer);
    aiv_vector_add(engine->spriteSheets, spriteSheet);
    spriteSheet = CreateSpriteSheet("resources/assets/enemy/explosion1_strip6.png", 6, 0.1f, EnemyExplosionA, engine->renderer);
    aiv_vector_add(engine->spriteSheets, spriteSheet);
    spriteSheet = CreateSpriteSheet("resources/assets/player/myplane_strip3.png", 3, 0.3f, PlayerA, engine->renderer);
    aiv_vector_add(engine->spriteSheets, spriteSheet);
    spriteSheet = CreateSpriteSheet("resources/assets/player/explosion2_strip7.png", 7, 0.1f, PlayerExplosionA, engine->renderer);
    aiv_vector_add(engine->spriteSheets, spriteSheet);
    spriteSheet = CreateSpriteSheet("resources/assets/player/smoke_strip10.png", 10, 0.2f, SmokeA, engine->renderer);
    aiv_vector_add(engine->spriteSheets, spriteSheet);
}

//USE THIS TO SET A SINGLE SPRITE TO A RENDER COMPONENT
struct Sprite GetSprite(struct GFXEngine *engine, SpriteType type)
{
    struct Sprite *sprite = aiv_vector_at(engine->sprites, (int)type);
    if (sprite == NULL)
        sprite = aiv_vector_at(engine->sprites, NullSprite);
    struct Sprite s = *sprite;
    return s;
}

//USE THIS TO SET A SPRITESHEET TO AN ANIMATOR COMPONENT
struct SpriteSheet GetAnimation(struct GFXEngine *engine, AnimationType type)
{
    struct SpriteSheet *spriteSheet = aiv_vector_at(engine->spriteSheets, (int)type);
    if (spriteSheet == NULL)
        spriteSheet = aiv_vector_at(engine->spriteSheets, NullAnimation);
    struct SpriteSheet s = *spriteSheet;
    return s;
}

void CreateFonts(struct GFXEngine* engine)
{
    FC_Font* font = FC_CreateFont();
    FC_LoadFont(font, engine->renderer, "resources/assets/fonts/Haettenschweiler.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    aiv_vector_add(engine->fonts, font);
}

FC_Font* GetFont(struct GFXEngine* engine, FontType font)
{
    return aiv_vector_at(engine->fonts, font);
}