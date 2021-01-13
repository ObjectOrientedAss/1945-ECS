#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <SDL_image.h>
#include "aiv-vector.h"
#include "sdl_font_cache.h"

//the fonts list
enum f_type
{
    Haettenschweiler
};
typedef enum f_type FontType;

//the animations list
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

//the sprites list
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
    LifePowerupS,
    SplashS
};
typedef enum s_type SpriteType;

//basic structure containing the origin rect to read a texture, the destination rect to render, and the actual texture
struct Sprite
{
    SDL_Rect originRect; //should match the single frame width and height, for the X position, combine it with the current frame index
    SDL_Rect spriteRect; //the rendering size and position of the rect to show on the screen (destination rect)
    SDL_Texture* texture; //the texture to render
};

//basic structure containing the spritesheet texture, and all the data to manage the animation
struct SpriteSheet
{
    SDL_Texture *texture;   //the texture of the spritesheet
    int frameWidth;         //the width of a single frame of the current texture
    int frameHeight;        //the height of a single frame of the current texture
    int frames;             //the number of frames of the current texture
    AnimationType type;     //the animation type, useful to pick and set a spritesheet quickly
    float baseFrameDuration;//the base time that should pass between a frame and the next one on this spritesheet
};

//the engine containing all the informations about the window, the renderer, and the graphics stuff like sprite, spritesheets and fonts
struct GFXEngine
{
    int windowWidth;                //the width of the window, queried at start
    int windowHeight;               //the height of the window, queried at start
    SDL_Window *window;             //the SDL window
    SDL_Renderer *renderer;         //the main SDL renderer
    aiv_vector *spriteSheets;       //all the multi sprite textures are contained here
    aiv_vector *sprites;            //all the single sprite textures are contained here
    aiv_vector *fonts;              //all the fonts are contained here
};

//start and allocate the graphics engine and all the gfx resources
struct GFXEngine *__GFXEngineInit();
//stop and deallocate the graphics engine and all the gfx resources
void __StopGFXEngine(struct GFXEngine* engine);
//create a spritesheet of the requested type
struct SpriteSheet *__CreateSpriteSheet(char *path, int frames, float baseFrameDuration, AnimationType type, SDL_Renderer* renderer);
//create a sprite of the requested type
struct Sprite* __CreateSprite(char* path, SpriteType type, SDL_Renderer* renderer);
//generate all the spritesheets needed
void __LoadSpriteSheets(struct GFXEngine *engine);
//generate all the sprites needed
void __LoadSprites(struct GFXEngine *engine);
//get the sprite of the requested type from the resources
struct Sprite GetSprite(struct GFXEngine* engine, SpriteType type);
//get the spritesheet of the requested type from the resources
struct SpriteSheet GetAnimation(struct GFXEngine* engine, AnimationType type);
//create the needed fonts
void __CreateFonts(struct GFXEngine* engine);
//get a font of the requested type to render text on screen
FC_Font* GetFont(struct GFXEngine* engine, FontType font);

#endif //GRAPHICS_ENGINE_H