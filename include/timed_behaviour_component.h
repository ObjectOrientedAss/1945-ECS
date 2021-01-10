#ifndef TIMED_BEHAVIOUR_COMPONENT_H
#define TIMED_BEHAVIOUR_COMPONENT_H
#include "ecs.h"
#include "aiv-vector.h"

//TIMED COMPONENT BEHAVIOUR: should constantly update a timer and perform an action when it expires
struct TimedBehaviourComponent
{
    int currentRepetitions;
    int repetitions;
    double time;
    double elapsedTime;
    aiv_vector* customArgs;
};

//INIT
void InitTimedBehaviourComponent(struct TimedBehaviourComponent *timedBehaviourComponent, int repetitions, float time, aiv_vector* customArgs);

//UPDATE
void AlphaBlendingBehaviour(struct Component *selfComponent, struct Game *game);
void ReleaseSmokeBehaviour(struct Component *selfComponent, struct Game *game);
void DetachAttackPowerupBehaviour(struct Component *selfComponent, struct Game *game);
void DetachSpeedPowerupBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnPowerupBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnIslandBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnEnemyBehaviour(struct Component *selfComponent, struct Game *game);
void SpawnEnemySquadronBehaviour(struct Component *selfComponent, struct Game *game);
void ToggleEntityAfterBehaviour(struct Component *selfComponent, struct Game *game);
void PlayerTimedRespawnBehaviour(struct Component *selfComponent, struct Game *game);
void ScoreDistanceBehaviour(struct Component *selfComponent, struct Game *game);
void GoToMainMenuAfter(struct Component *selfComponent, struct Game *game);
void OndulationBehaviour(struct Component *selfComponent, struct Game *game);

#endif //TIMED_BEHAVIOUR_COMPONENT_H