#ifndef ANIMATOR_COMPONENT_H
#define ANIMATOR_COMPONENT_H
#include "ecs.h"

//ANIMATOR COMPONENT BEHAVIOUR: should set and update a current animation to change the sprite rendered by the RenderComponent
struct AnimatorComponent
{
    double frameDuration;
    double timeElapsed;
    struct GFXEngine* engine;
    struct SpriteSheet currentAnimation;
    int currentFrameIndex;
    void (*SetAnimation)(struct Component *selfComponent, AnimationType type, float frameDuration);
};

//INIT
void InitAnimatorComponent(struct Component *animatorComponent, struct GFXEngine *engine, AnimationType animationType, float frameDuration, void (*SetAnimation)(struct Component *selfComponent, AnimationType type, float frameDuration));

//UPDATE
void AnimatorBehaviour(struct Component *selfComponent, struct Game* game);

//CUSTOM
void SetAnimation(struct Component *selfComponent, AnimationType type, float frameDuration);

#endif //ANIMATOR_COMPONENT_H