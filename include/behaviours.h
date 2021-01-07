#ifndef BEHAVIOURS_H
#define BEHAVIOURS_H

#include "SDL.h"
#include "ecs.h"
#include "game.h"

//COMPONENTS SYSTEM BEHAVIOUR METHODS (Update)----------------------------------------------------------------------------

//TIMED COMPONENT
void AlphaBlendingBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnIslandBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnEnemyBehaviour(struct Component *selfComponent, struct Game *game);
void ToggleEntityAfterBehaviour(struct Component *selfComponent, struct Game *game);
void PlayerTimedRespawnBehaviour(struct Component *selfComponent, struct Game *game);
void GoToMainMenuAfter(struct Component *selfComponent, struct Game *game);
//INPUT COMPONENT
void InputBehaviour(struct Component *selfComponent, struct Game* game);
//SHOOT COMPONENT
void EnemyShootBehaviour(struct Component* selfComponent, struct Game* game);
void PlayerShootBehaviour(struct Component* selfComponent, struct Game* game);
//BUTTON COMPONENT
void ButtonBehaviour(struct Component *selfComponent, struct Game* game);
//MOVEMENT COMPONENT
void WaterMovementBehaviour(struct Component* selfComponent, struct Game* game);
void PlayerMovementBehaviour(struct Component *selfComponent, struct Game* game);
void AutomatedMovementBehaviour(struct Component *selfComponent, struct Game* game);
//PHYSICS COMPONENT
void PhysicsBehaviour(struct Component *selfComponent, struct Game* game);
//HEALTH COMPONENT
void HealthBehaviour(struct Component *selfComponent, struct Game* game);
//ANIMATOR COMPONENT
void AnimatorBehaviour(struct Component *selfComponent, struct Game* game);
//AUDIO COMPONENT
void AudioBehaviour(struct Component *selfComponent, struct Game *game);
//RENDER COMPONENT
void RenderBehaviour(struct Component *selfComponent, struct Game* game);

//COMPONENTS INNER METHODS (Custom)---------------------------------------------------------------------------------------

void PlayerDeath(struct Component* selfComponent, struct Game* game);
void EnemyDeath(struct Component* selfComponent, struct Game* game);
void Collide(struct Component *selfComponent, struct Component *otherComponent);
void ChangeHealth(HealthComponent *selfComponent, float amount);
void SetAnimation(struct Component *selfComponent, AnimationType type, float frameDuration);
void ButtonHoverStart(struct Component* selfComponent);
void ButtonHoverEnd(struct Component* selfComponent);
void StartGameClick(struct Component* selfComponent, struct Game* game);
void QuitGameClick(struct Component* selfComponent, struct Game* game);
void SetAudio(struct Component* selfComponent, AudioType type, int loops);

//COMPONENTS INIT METHODS (Awake)----------------------------------------------------------------------------------------

void InitTimedBehaviourComponent(TimedBehaviourComponent *timedBehaviourComponent, int repetitions, float time, aiv_vector* customArgs);
void InitParticleComponent(struct Component* selfComponent, void(*PlayParticle)(AnimationType type));
void InitShootComponent(ShootComponent* selfComponent, float shootCooldown, EntityType bulletType);
void InitMovementComponent(MovementComponent* movementComponent, vec2 velocity, float speed);
void InitButtonComponent(struct Component* buttonComponent, void(*OnClick)(struct Component* selfComponent, struct Game* game), void(*OnHoverStart)(struct Component *selfComponent), void(*OnHoverEnd)(struct Component *selfComponent));
void InitPhysicsComponent(struct Component *selfComponent, void (*Collide)(struct Component* selfComponent, struct Component* otherComponent));
void InitRenderComponent(RenderComponent *renderComponent, struct GFXEngine *engine, SpriteType spriteType);
void InitAnimatorComponent(struct Component *animatorComponent, struct GFXEngine *engine, AnimationType animationType, float frameDuration, void (*SetAnimation)(struct Component *selfComponent, AnimationType type, float frameDuration));
void InitAudioComponent(struct Component *selfComponent, struct AudioEngine *engine, AudioType audioType, int loops, void (*SetAudio)(struct Component* selfComponent, AudioType type, int loops));
void InitTransformComponent(TransformComponent* transformComponent, vec2 position);
void InitHealthComponent(HealthComponent *healthComponent, int maxLives, int startingLives, float maxHealth, float startingHealth, void (*ChangeHealth)(HealthComponent *selfComponent, float amount), void (*Die)(struct Component *selfComponent, struct Game *game));

#endif //BEHAVIOURS_H