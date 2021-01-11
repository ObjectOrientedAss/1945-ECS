#include "physics_component.h"

void InitPhysicsComponent(struct Component *selfComponent, void (*Collide)(struct Component *selfComponent, struct Component *otherComponent, struct Game *game))
{
    struct PhysicsComponent *physicsComponent = selfComponent->data;
    struct RenderComponent *renderComponent = GetComponentData(selfComponent->owner, RenderC);
    if (renderComponent == NULL)
        physicsComponent->colliderRadius = 10;
    else
        physicsComponent->colliderRadius = renderComponent->sprite.spriteRect.w * 0.5f;

    physicsComponent->canCollide = true;
    physicsComponent->Collide = Collide;

    physicsComponent->collisionsBlockTime = 0.08f;
    physicsComponent->collisionsBlockTimeElapsed = 0;

    //SETUP COLLISION MASKS
    switch (selfComponent->owner->family)
    {
    case PlayerAirplane:
        physicsComponent->layersBitmask = ENEMY_COLLISION_LAYER | ENEMYBULLET_COLLISION_LAYER | POWERUP_COLLISION_LAYER;
        physicsComponent->selfLayer = PLAYER_COLLISION_LAYER;
        break;
    case EnemyAirplane:
        physicsComponent->layersBitmask = PLAYER_COLLISION_LAYER | PLAYERBULLET_COLLISION_LAYER;
        physicsComponent->selfLayer = ENEMY_COLLISION_LAYER;
        break;
    case BossAirplane:
        physicsComponent->layersBitmask = PLAYER_COLLISION_LAYER | PLAYERBULLET_COLLISION_LAYER;
        physicsComponent->selfLayer = ENEMY_COLLISION_LAYER;
        break;
    case PlayerBullet:
        physicsComponent->layersBitmask = ENEMY_COLLISION_LAYER;
        physicsComponent->selfLayer = PLAYERBULLET_COLLISION_LAYER;
        break;
    case EnemyBullet:
        physicsComponent->layersBitmask = PLAYER_COLLISION_LAYER;
        physicsComponent->selfLayer = ENEMYBULLET_COLLISION_LAYER;
        break;
    case BossBullet:
        physicsComponent->layersBitmask = PLAYER_COLLISION_LAYER;
        physicsComponent->selfLayer = ENEMYBULLET_COLLISION_LAYER;
        break;
    case SpeedPowerup:
        physicsComponent->layersBitmask = PLAYER_COLLISION_LAYER;
        physicsComponent->selfLayer = POWERUP_COLLISION_LAYER;
    case AttackPowerup:
        physicsComponent->layersBitmask = PLAYER_COLLISION_LAYER;
        physicsComponent->selfLayer = POWERUP_COLLISION_LAYER;
    case LifePowerup:
        physicsComponent->layersBitmask = PLAYER_COLLISION_LAYER;
        physicsComponent->selfLayer = POWERUP_COLLISION_LAYER;
    }
}

void Collide(struct Component *selfComponent, struct Component *otherComponent, struct Game *game)
{
    //get my physics component
    struct PhysicsComponent *selfPhysicsComponent = selfComponent->data;
    //get other physics component
    struct PhysicsComponent *otherPhysicsComponent = otherComponent->data;

    struct BattleLevelData *bld = game->levelData;

    switch (selfComponent->owner->type)
    {
    case Airplane: //i am an airplane
        switch (selfComponent->owner->family)
        {
        case PlayerAirplane:                                    //specifically, i am a player airplane
            if (otherComponent->owner->family == EnemyAirplane) //other is an enemy airplane
            {
                struct HealthComponent *healthComponent = GetComponentData(selfComponent->owner, HealthC);
                healthComponent->ChangeHealth(healthComponent, -bld->playerSuicideDamage);
            }
            else if (otherComponent->owner->family == BossAirplane) //other is an enemy airplane
            {
                struct HealthComponent *healthComponent = GetComponentData(selfComponent->owner, HealthC);
                healthComponent->ChangeHealth(healthComponent, -bld->playerSuicideDamage);
            }
            else if (otherComponent->owner->family == EnemyBullet) //other is an enemy bullet
            {
                struct HealthComponent *healthComponent = GetComponentData(selfComponent->owner, HealthC);
                healthComponent->ChangeHealth(healthComponent, -bld->enemyBulletsDamage);
            }
            else if (otherComponent->owner->family == BossBullet)
            {
                struct HealthComponent *healthComponent = GetComponentData(selfComponent->owner, HealthC);
                healthComponent->ChangeHealth(healthComponent, -bld->bossBulletsDamage);
            }
            else if (otherComponent->owner->family == AttackPowerup) //other is an attack pu
            {
                //attack powerup attach
                bld->playerBulletsDamage *= bld->attackPowerupBonus;
                struct Component *tb = AddComponent(selfComponent->owner, TimedBehaviourC, DetachAttackPowerupBehaviour);
                InitTimedBehaviourComponent(tb->data, 1, bld->powerupDuration, NULL);
                bld->powerupUI->text = "Increased Attack!";
            }
            else if (otherComponent->owner->family == SpeedPowerup) //other is a speed pu
            {
                //speed powerup attach
                bld->playerSpeed *= bld->speedPowerupBonus;
                struct Component *tb = AddComponent(selfComponent->owner, TimedBehaviourC, DetachSpeedPowerupBehaviour);
                InitTimedBehaviourComponent(tb->data, 1, bld->powerupDuration, NULL);
                bld->powerupUI->text = "Increased Speed!";
            }
            else if (otherComponent->owner->family == LifePowerup) //other is a life pu
            {
                struct HealthComponent *healthComponent = GetComponentData(selfComponent->owner, HealthC);
                if (healthComponent->currentLives < healthComponent->maxLives)
                {
                    struct Entity *lifeUI = aiv_vector_at(bld->lives, healthComponent->currentLives);
                    SetEntityActiveStatus(lifeUI, true);
                    healthComponent->currentLives++;
                }
            }
            break;
        case EnemyAirplane:                                      //specifically i am an enemy airplane
            if (otherComponent->owner->family == PlayerAirplane) //other is a player airplane
            {
                struct HealthComponent *healthComponent = GetComponentData(selfComponent->owner, HealthC);
                healthComponent->ChangeHealth(healthComponent, -healthComponent->maxHealth);
            }
            else if (otherComponent->owner->family == PlayerBullet) //other is a player bullet
            {
                struct HealthComponent *healthComponent = GetComponentData(selfComponent->owner, HealthC);
                healthComponent->ChangeHealth(healthComponent, -bld->playerBulletsDamage);
            }
            break;
        case BossAirplane:                                     //specifically i am an enemy airplane
            if (otherComponent->owner->family == PlayerBullet) //other is a player bullet
            {
                struct HealthComponent *healthComponent = GetComponentData(selfComponent->owner, HealthC);
                healthComponent->ChangeHealth(healthComponent, -bld->playerBulletsDamage);
            }
            break;
        }
        break;
    case Bullet:
        switch (selfComponent->owner->family)
        {
        case PlayerBullet:
            if (otherComponent->owner->family == EnemyAirplane || otherComponent->owner->family == BossAirplane)
                SetEntityActiveStatus(selfComponent->owner, false);
            break;
        case EnemyBullet:
            if (otherComponent->owner->family == PlayerAirplane)
                SetEntityActiveStatus(selfComponent->owner, false);
            break;
        case BossBullet:
            if (otherComponent->owner->family == PlayerAirplane)
                SetEntityActiveStatus(selfComponent->owner, false);
            break;
        }
        break;
    case Powerup:
        if (otherComponent->owner->family == PlayerAirplane)
            SetEntityActiveStatus(selfComponent->owner, false);
    }
}

void PhysicsBehaviour(struct Component *selfComponent, struct Game *game)
{
    //get my physics component
    struct PhysicsComponent *physicsComponent = selfComponent->data;

    if (!physicsComponent->canCollide)
    {
        physicsComponent->collisionsBlockTimeElapsed += game->engine->time->deltaTimeInSeconds; //the time passed since the last collision
        if (physicsComponent->collisionsBlockTimeElapsed >= physicsComponent->collisionsBlockTime)
        {
            physicsComponent->collisionsBlockTimeElapsed = 0;
            physicsComponent->canCollide = true;
        }
        else
            return;
    }

    //get my movement component
    struct TransformComponent *tc = GetComponentData(selfComponent->owner, TransformC);

    //iterate all the other physics components!
    aiv_vector *physicsSystem = aiv_vector_at(selfComponent->owner->ECS->systems, selfComponent->type);
    for (uint i = 0; i < aiv_vector_size(physicsSystem); i++)
    {
        struct Component *otherComponent = aiv_vector_at(physicsSystem, i);
        if (!otherComponent->owner->active)
            continue;

        //get the other physics component
        struct PhysicsComponent *otherPhysicsComponent = otherComponent->data;
        if (!otherPhysicsComponent->canCollide) //the other collider has already collided in this frame!
            continue;
        if (physicsComponent == otherPhysicsComponent) //i am also in the same list, so, if this is myself...
            continue;

        //check if the collision layers are compatible
        if ((physicsComponent->selfLayer & otherPhysicsComponent->layersBitmask))
        {
            //get the other transform component and relative position
            struct TransformComponent *otherTransformComponent = GetComponentData(otherComponent->owner, TransformC);
            //get the distance vector between me and the other
            vec2 vDist = vec2_new(0, 0);
            vDist = vec2_sub(&tc->position, &otherTransformComponent->position);
            //if the distance is less than the two colliders radiuses combined, we are colliding on each other
            if (vec2_magn(&vDist) <= physicsComponent->colliderRadius + otherPhysicsComponent->colliderRadius)
            {
                //handle the collision on me:
                physicsComponent->canCollide = false;
                physicsComponent->Collide(selfComponent, otherComponent, game);
                //trigger and handle the collision on the other:
                otherPhysicsComponent->canCollide = false;
                otherPhysicsComponent->Collide(otherComponent, selfComponent, game);
                break; //i just want to be able to collide once per frame, so after colliding, stop checking collisions on this object!
            }
        }
    }
}