#ifndef ANIMATOR_COMPONENT_H
#define ANIMATOR_COMPONENT_H
#include "ecs.h"

//ANIMATOR SYSTEM COMPONENT
struct AnimatorComponent
{
    double frameDuration; //the duration between a frame of the spritesheet and the next one
    double timeElapsed;   //the time passed since the last spritesheet frame switch
    struct GFXEngine* engine;   //the graphics engine from which to take the renderer
    struct SpriteSheet currentAnimation;    //the current spritesheet processed by the animator
    int currentFrameIndex;  //the current frame index rendered by the spritesheet
    void (*SetAnimation)(struct Component *selfComponent, AnimationType type, float frameDuration); //the function called whenever a new spritesheet must be set
};

//INIT
void InitAnimatorComponent(struct Component *animatorComponent, struct GFXEngine *engine, AnimationType animationType, float frameDuration, void (*SetAnimation)(struct Component *selfComponent, AnimationType type, float frameDuration));

//UPDATE
void AnimatorBehaviour(struct Component *selfComponent, struct Game* game);

//CUSTOM
void SetAnimation(struct Component *selfComponent, AnimationType type, float frameDuration);

#endif //ANIMATOR_COMPONENT_H