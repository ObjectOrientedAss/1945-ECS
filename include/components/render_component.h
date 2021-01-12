#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H
#include "ecs.h"

//RENDER SYSTEM COMPONENT
struct RenderComponent
{
    struct Sprite sprite;   //the current sprite to be rendered, it can be a single sprite set or a spritesheet frame
    SDL_Renderer* renderer;  //the renderer on which we want to render the sprite over
};

//INIT
void InitRenderComponent(struct Component *selfComponent, struct GFXEngine *engine, SpriteType spriteType);

//UPDATE
void RenderBehaviour(struct Component *selfComponent, struct Game* game);
void FontRenderBehaviour(struct Component *selfComponent, struct Game *game);

#endif //RENDER_COMPONENT_H