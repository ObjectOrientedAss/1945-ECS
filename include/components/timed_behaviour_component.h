#ifndef TIMED_BEHAVIOUR_COMPONENT_H
#define TIMED_BEHAVIOUR_COMPONENT_H
#include "ecs.h"
#include "aiv-vector.h"

//TIMED SYSTEM COMPONENT
struct TimedBehaviourComponent
{
    int repetitions;            //the number of times we want to repeat the countdown on this behaviour
    int currentRepetitions;     //the current number of repetitions done by this component
    double time;                //the countdown timer to update each frame
    double elapsedTime;         //the countdown timer elapsed
    aiv_vector* customArgs;     //a vector containing a list of custom void* arguments to be casted into the component if needed
};

//INIT
void InitTimedBehaviourComponent(struct TimedBehaviourComponent *timedBehaviourComponent, int repetitions, float time, aiv_vector* customArgs);

//UPDATE
void SplashScreenBehaviour(struct Component *selfComponent, struct Game *game);
void AlphaBlendingBehaviour(struct Component *selfComponent, struct Game *game);
void ReleaseSmokeBehaviour(struct Component *selfComponent, struct Game *game);
void DetachAttackPowerupBehaviour(struct Component *selfComponent, struct Game *game);
void DetachSpeedPowerupBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnPowerupBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnIslandBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnBossBehaviour(struct Component *selfComponent, struct Game *game);
void BossSwitchShootBehaviour(struct Component *selfComponent, struct Game *game);
void BossExplosionBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnEnemyBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnEnemySquadronBehaviour(struct Component *selfComponent, struct Game *game);
void ToggleEntityAfterBehaviour(struct Component *selfComponent, struct Game *game);
void PlayerTimedRespawnBehaviour(struct Component *selfComponent, struct Game *game);
void ScoreDistanceBehaviour(struct Component *selfComponent, struct Game *game);
void GoToMainMenuAfter(struct Component *selfComponent, struct Game *game);
void OndulationBehaviour(struct Component *selfComponent, struct Game *game);

#endif //TIMED_BEHAVIOUR_COMPONENT_H