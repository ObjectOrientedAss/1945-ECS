#ifndef BEHAVIOURS_H
#define BEHAVIOURS_H

#include "SDL.h"
#include "ecs.h"
#include "game.h"

//COMPONENTS SYSTEM BEHAVIOUR METHODS----------------------------------------------------------------------------

void InputBehaviour(struct Component *selfComponent, void* game);
void ButtonBehaviour(struct Component *selfComponent, void* game);
void PlayerMovementBehaviour(struct Component *selfComponent, void* game);
void EnemyMovementBehaviour(struct Component *selfComponent, void* game);
void PhysicsBehaviour(struct Component *selfComponent, void* game);
void HealthBehaviour(struct Component *selfComponent, void* game);
void AnimatorBehaviour(struct Component *selfComponent, void* game);
void RenderBehaviour(struct Component *selfComponent, void* game);

//COMPONENTS INNER METHODS---------------------------------------------------------------------------------------

void Collide(struct Component *selfComponent, struct Component *otherComponent);
void ChangeHealth(HealthComponent *selfComponent, float amount);
void SetAnimation(struct Component *selfComponent, AnimationType type, float frameDuration);
void ButtonHoverStart(struct Component* selfComponent);
void ButtonHoverEnd(struct Component* selfComponent);
void StartGameClick(struct Component* selfComponent, struct Game* game);
void QuitGameClick(struct Component* selfComponent, struct Game* game);

//COMPONENTS INIT METHODS----------------------------------------------------------------------------------------

void InitMovementComponent(MovementComponent* movementComponent, vec2 velocity, float speed);
void InitTransformComponent(TransformComponent* transformComponent, vec2 position);
void InitButtonComponent(struct Component* buttonComponent, void(*OnClick)(struct Component* selfComponent, struct Game* game), vec2 position);
void InitPhysicsComponent(struct Component *selfComponent);
void InitRenderComponent(RenderComponent *renderComponent, struct GFXEngine *engine, SpriteType spriteType);
void InitAnimatorComponent(AnimatorComponent *animatorComponent, struct GFXEngine *engine, AnimationType animationType);

#endif //BEHAVIOURS_H