#include "ui_component.h"

void InitUIComponent(struct Component *selfComponent, void (*OnClick)(struct Component *selfComponent, struct Game *game), void (*OnHoverStart)(struct Component *selfComponent), void (*OnHoverEnd)(struct Component *selfComponent))
{
    struct UIComponent *buttonComponent = selfComponent->data;
    buttonComponent->OnClick = OnClick;
    buttonComponent->OnHoverStart = OnHoverStart;
    buttonComponent->OnHoverEnd = OnHoverEnd;
}

void UIBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct UIComponent *uiComponent = selfComponent->data;

    struct RenderComponent *renderComponent = GetComponentData(selfComponent->owner, RenderC);
    struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);

    //if the mouse is over the button
    if (game->mousePositionX >= tc->position.x - (renderComponent->sprite.spriteRect.w * 0.5f) &&
        game->mousePositionX <= tc->position.x + (renderComponent->sprite.spriteRect.w * 0.5f) &&
        game->mousePositionY >= tc->position.y - (renderComponent->sprite.spriteRect.h * 0.5f) &&
        game->mousePositionY <= tc->position.y + (renderComponent->sprite.spriteRect.h * 0.5f))
    {
        //if it is the first frame in which the mouse has started hovering
        if (!uiComponent->isHovering)
        {
            uiComponent->isHovering = true;
            if (uiComponent->OnHoverStart != NULL)
                uiComponent->OnHoverStart(selfComponent);
        }
        else
        {
            if (game->mouseDown)
                uiComponent->OnClick(selfComponent, game);
        }
    }
    else //if the mouse is not over the button
    {
        if (uiComponent->isHovering)
        {
            uiComponent->isHovering = false;
            if (uiComponent->OnHoverEnd != NULL)
                uiComponent->OnHoverEnd(selfComponent);
        }
    }
}

void ButtonHoverStart(struct Component *selfComponent)
{
    struct RenderComponent *rc = GetComponentData(selfComponent->owner, RenderC);
    rc->sprite.spriteRect.w += 10;
    rc->sprite.spriteRect.h += 10;
}

//
void ButtonHoverEnd(struct Component *selfComponent)
{
    struct RenderComponent *rc = GetComponentData(selfComponent->owner, RenderC);
    rc->sprite.spriteRect.w -= 10;
    rc->sprite.spriteRect.h -= 10;
}

//the start button click event in the main menu
void StartGameClick(struct Component *selfComponent, struct Game *game)
{
    game->sceneToLoad = GameScene;
}

//the quit button click event in the main menu
void QuitGameClick(struct Component *selfComponent, struct Game *game)
{
    game->quitRequest = true;
}