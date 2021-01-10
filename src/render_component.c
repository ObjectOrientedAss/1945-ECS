#include "render_component.h"

void RenderBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Render Behaviour Called\n");
    struct RenderComponent *renderComponent = selfComponent->data;

    struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);
    renderComponent->sprite.spriteRect.x = (int)tc->position.x - (int)(renderComponent->sprite.spriteRect.w * 0.5f);
    renderComponent->sprite.spriteRect.y = (int)tc->position.y - (int)(renderComponent->sprite.spriteRect.h * 0.5f);

    SDL_RenderCopy(renderComponent->renderer, renderComponent->sprite.texture, &renderComponent->sprite.originRect, &renderComponent->sprite.spriteRect);
}

void FontRenderBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct RenderComponent *renderComponent = selfComponent->data;

    struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);
    renderComponent->sprite.spriteRect.x = (int)tc->position.x - (int)(renderComponent->sprite.spriteRect.w * 0.5f);
    renderComponent->sprite.spriteRect.y = (int)tc->position.y - (int)(renderComponent->sprite.spriteRect.h * 0.5f);

    struct TextComponent *textComponent = GetComponentData(selfComponent->owner, TextC);
    FC_Draw(GetFont(game->engine->GfxEngine, textComponent->fontType), renderComponent->renderer, renderComponent->sprite.spriteRect.x, renderComponent->sprite.spriteRect.y, textComponent->text);
}

void InitRenderComponent(struct RenderComponent *renderComponent, struct GFXEngine *engine, SpriteType spriteType)
{
    renderComponent->renderer = engine->renderer;
    renderComponent->sprite = GetSprite(engine, spriteType);
    // printf("\n\n-------------------------------------------------\n\n");
    // printf("\n\nOR H: %d | OR W: %d | OR X: %d | OR Y: %d", renderComponent->sprite.originRect.h, renderComponent->sprite.originRect.w, renderComponent->sprite.originRect.x, renderComponent->sprite.originRect.y);
    // printf("\n\nSR H: %d | SR W: %d | SR X: %d | SR Y: %d", renderComponent->sprite.spriteRect.h, renderComponent->sprite.spriteRect.w, renderComponent->sprite.spriteRect.x, renderComponent->sprite.spriteRect.y);
    printf("\n---Render Component Initialized!");
}