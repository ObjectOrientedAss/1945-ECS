#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H
#include "ecs.h"

//RENDER COMPONENT BEHAVIOUR: should render the entity sprite
struct RenderComponent
{
    struct Sprite sprite;
    SDL_Renderer* renderer;
};

//INIT
void InitRenderComponent(struct RenderComponent *renderComponent, struct GFXEngine *engine, SpriteType spriteType);

//UPDATE
void RenderBehaviour(struct Component *selfComponent, struct Game* game);
void FontRenderBehaviour(struct Component *selfComponent, struct Game *game);

#endif //RENDER_COMPONENT_H