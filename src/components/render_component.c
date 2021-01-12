#include "render_component.h"

void RenderBehaviour(struct Component *selfComponent, struct Game *game)
{
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

void InitRenderComponent(struct Component *selfComponent, struct GFXEngine *engine, SpriteType spriteType)
{
    struct RenderComponent* rc = selfComponent->data;
    rc->renderer = engine->renderer;
    rc->sprite = GetSprite(engine, spriteType);

    struct TransformComponent* tc = GetComponentData(selfComponent->owner, TransformC);
    rc->sprite.spriteRect.w *= tc->scale.x;
    rc->sprite.spriteRect.h *= tc->scale.y;
}