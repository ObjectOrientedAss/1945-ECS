#include "animator_component.h"

void AnimatorBehaviour(struct Component *selfComponent, struct Game *game)
{
    struct AnimatorComponent *animatorComponent = (struct AnimatorComponent*)selfComponent->data;

    if (animatorComponent->currentAnimation.frames <= 1) //if an asshole has created an animation spritesheet with 1 or less frames...
        return;

    struct RenderComponent *renderComponent = (struct RenderComponent*)GetComponentData(selfComponent->owner, RenderC);

    animatorComponent->timeElapsed += game->engine->time->deltaTimeInSeconds;
    if (animatorComponent->timeElapsed >= animatorComponent->frameDuration)
    {
        animatorComponent->timeElapsed = 0;
        animatorComponent->currentFrameIndex++;
        if (animatorComponent->currentFrameIndex >= animatorComponent->currentAnimation.frames)
            animatorComponent->currentFrameIndex = 0;

        renderComponent->sprite.originRect.x = animatorComponent->currentFrameIndex * animatorComponent->currentAnimation.frameWidth;
    }
}

//Set the animation to perform on the given animator component.
//If you also want to alter the frame duration of the spritesheet, pass a value > 0.
void SetAnimation(struct Component *selfComponent, AnimationType type, float frameDuration)
{
    struct AnimatorComponent *animatorComponent = selfComponent->data;

    struct RenderComponent *renderComponent = GetComponentData(selfComponent->owner, RenderC);

    animatorComponent->currentAnimation = GetAnimation(animatorComponent->engine, type);
    animatorComponent->frameDuration = animatorComponent->currentAnimation.baseFrameDuration;
    animatorComponent->timeElapsed = 0;
    animatorComponent->currentFrameIndex = 0;
    if (frameDuration > 0)
        animatorComponent->frameDuration = frameDuration;

    renderComponent->sprite.texture = animatorComponent->currentAnimation.texture;
    renderComponent->sprite.originRect.w = animatorComponent->currentAnimation.frameWidth;
    renderComponent->sprite.originRect.h = animatorComponent->currentAnimation.frameHeight;
    renderComponent->sprite.originRect.x = animatorComponent->currentFrameIndex * animatorComponent->currentAnimation.frameWidth;

    struct TransformComponent* tc = GetComponentData(selfComponent->owner, TransformC);
    renderComponent->sprite.spriteRect.w = animatorComponent->currentAnimation.frameWidth * tc->scale.x;
    renderComponent->sprite.spriteRect.h = animatorComponent->currentAnimation.frameHeight * tc->scale.y;
}

void InitAnimatorComponent(struct Component *selfComponent, struct GFXEngine *engine, AnimationType animationType, float frameDuration, void (*SetAnimation)(struct Component *selfComponent, AnimationType type, float frameDuration))
{
    struct AnimatorComponent *animatorComponent = selfComponent->data;
    animatorComponent->engine = engine;
    animatorComponent->SetAnimation = SetAnimation;
    animatorComponent->SetAnimation(selfComponent, animationType, frameDuration);
}