#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <SDL_image.h>
#include "aiv-vector.h"
#include "sdl_font_cache.h"

enum f_type
{
    Haettenschweiler
};
typedef enum f_type FontType;

enum a_type
{
    NullAnimation,
    Enemy1A,
    Enemy2A,
    Enemy3A,
    Enemy4A,
    EnemyExplosionA,
    PlayerA,
    PlayerExplosionA,
    SmokeA,
    BossA
};
typedef enum a_type AnimationType;

enum s_type
{
    NullSprite,
    StartButtonS,
    QuitButtonS,
    EnemyBullet1S,
    EnemyBullet2S,
    PlayerBulletS,
    TitleS,
    WaterS,
    Island1S,
    Island2S,
    Island3S,
    BottomS,
    LifeS,
    AttackPowerupS,
    SpeedPowerupS,
    LifePowerupS
};
typedef enum s_type SpriteType;

struct Sprite
{
    SDL_Rect originRect; //should match the single frame width and height, for the X position, combine it with the current frame index
    SDL_Rect spriteRect; //the rendering size and position of the rect to show on the screen
    SDL_Texture* texture; //the big texture (could be a spritesheet)
};

struct SpriteSheet
{
    SDL_Texture *texture;
    int frameWidth;
    int frameHeight;
    int frames;
    AnimationType type;
    float baseFrameDuration;
};

struct GFXEngine
{
    int windowWidth;
    int windowHeight;
    SDL_Window *window;
    SDL_Renderer *renderer;
    aiv_vector *spriteSheets;       //all the multi sprite textures are contained here
    aiv_vector *sprites;            //all the single sprite textures are contained here
    aiv_vector *fonts;              //all the fonts are contained here
};

void StopGFXEngine(struct GFXEngine* engine);
struct SpriteSheet *CreateSpriteSheet(char *path, int frames, float baseFrameDuration, AnimationType type, SDL_Renderer* renderer);
struct Sprite* CreateSprite(char* path, SpriteType type, SDL_Renderer* renderer);
void LoadSpriteSheets(struct GFXEngine *engine);
void LoadSprites(struct GFXEngine *engine);
struct GFXEngine *StartGFXEngine();
struct Sprite GetSprite(struct GFXEngine* engine, SpriteType type);
struct SpriteSheet GetAnimation(struct GFXEngine* engine, AnimationType type);
void CreateFonts(struct GFXEngine* engine);
FC_Font* GetFont(struct GFXEngine* engine, FontType font);

#endif //GRAPHICS_ENGINE_H