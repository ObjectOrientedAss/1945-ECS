#ifndef ENTITY_H
#define ENTITY_H

#include "aiv-vector.h"

enum e_type
{
    NoType = -1,
    UI,
    Airplane,
    Bullet,
    Particle,
    Audio,
    Powerup,
    e_type_last
};
typedef enum e_type EntityType;

//The queue engine is automatically adapting to create an empty family queue for each family.
enum e_family
{
    NoFamily = -1,
    PlayerAirplane,
    EnemyAirplane,
    PlayerBullet,
    EnemyBullet,
    SmokeParticle,
    ExplosionParticle,
    SpeedPowerup,
    AttackPowerup,
    LifePowerup,
    Image,
    Button,
    Text,
    AudioEmitter,
    e_family_last
};
typedef enum e_family EntityFamily;

struct Entity
{
    uint __entityIndex;                //the index of this entity in the entities list of ECS, useful to find it or remove it immediately
    EntityType type;                   //useful to help macro-categorizing this entity and uniform some behaviours on it, like premade components to be attached all at once in creation
    EntityFamily family;               //useful to help specific-categorizing this entity and define it for specific behaviours
    aiv_vector *components;            //the list containing all the behaviours attached to this entity
    boolean active;                    //if this is true, the behaviours of this entity's system components CAN be called, else not
    boolean markedToDestroy;           //if this is true, the entity will be destroyed at the end of the current frame with all its components
    struct EntityComponentSystem *ECS; //the entity component system, useful to have access to all the systems lists at any time
};

#endif //ENTITY_H