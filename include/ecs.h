#ifndef ECS_H
#define ECS_H
#include <stdlib.h>
#include "aiv-vector.h"
#include "vec2.h"
#include "graphics_engine.h"
#include "audio_engine.h"

enum c_layer
{
    PLAYER_COLLISION_LAYER = 0x01,
    ENEMY_COLLISION_LAYER = 0x02,
    PLAYERBULLET_COLLISION_LAYER = 0x04,
    ENEMYBULLET_COLLISION_LAYER = 0x08,
    POWERUP_COLLISION_LAYER = 0x10
};
typedef enum c_layer CollisionLayer;

enum c_type
{
    //------DATA SYSTEMS------//
    TextC = -3,
    AudioC,
    TransformC,

    //^^^^^^DATA SYSTEMS^^^^^^//

    //------------------------//

    //------ECS  SYSTEMS------//

    TimedBehaviourC,
    InputC,
    ShootC,
    UIC,
    MovementC,
    PhysicsC,
    HealthC,
    AnimatorC,
    SoundC,
    RenderC,

    //^^^^^^ECS  SYSTEMS^^^^^^//
    
    c_type_last
};
typedef enum c_type ComponentType;

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

//ALL EXISTING COMPONENTS:
struct EntityComponentSystem
{
    aiv_vector *entitiesToDestroy;   //add an existing entity here when it is marked to be destroyed, remove it when it is destroyed
    aiv_vector *componentsToDestroy; //add an existing component here when it is marked to be destroyed, remove it when it is destroyed
    aiv_vector *entities;            //add an entity here when it is created, remove it when it is destroyed
    aiv_vector *systems;             //add a component to the right index in here when it is added to an entity, remove it when it or the owning entity are destroyed
};

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

struct Game;
struct Component
{
    int __systemIndex;                                                          //the index of this component in its system list, useful to find it or remove it immediately
    void *data;                                                                 //the real component defining its own behaviour
    struct Entity *owner;                                                       //the entity who owns this component
    ComponentType type;                                                         //useful to help finding the component in the list of components attached to an entity
    boolean active;                                                             //if this is true and the entity is also active, behaviour WILL be called, else not
    boolean markedToDestroy;                                                    //if this is true, the component will be destroyed at the end of the current frame
    void (*behaviour)(struct Component *selfComponent, struct Game* game);      //when the system updates this component, this behaviour is called, but only if active is true
};

//-----ECS SYSTEMS-----//
/*All the components of this category have a positive index in the ComponentType enum, and are constantly updated by the ECS.
They MUST have a behaviour, or when their system behaviour will be called in UpdateECS, they're gonna fire a nullptr.*/

//INPUT COMPONENT BEHAVIOUR: should register the inputs from an external device
typedef struct
{
    float xAxis;
    float yAxis;
    boolean shoot;
} InputComponent;

//SHOOT COMPONENT BEHAVIOUR: should spawn a bullet if the condition to shoot is valid
typedef struct
{
    double shootCooldown;
    double shootCooldownElapsed;
    EntityType bulletType;
} ShootComponent;

//TIMED COMPONENT BEHAVIOUR: should constantly update a timer and perform an action when it expires
typedef struct
{
    int currentRepetitions;
    int repetitions;
    double time;
    double elapsedTime;
    aiv_vector* customArgs;
} TimedBehaviourComponent;

//UI COMPONENT BEHAVIOUR: should check the user interactions with itself and perform the right callbacks
typedef struct 
{
    boolean isHovering;                                                 //set it to true when Hover Start is called
    void(*OnHoverStart)(struct Component *selfComponent);               //The behaviour to execute when the mouse starts hovering on this UI element.
    void(*OnHoverEnd)(struct Component *selfComponent);                 //The behaviour to execute when the mouse ends hovering on this UI element.
    void(*OnClick)(struct Component *selfComponent, struct Game* game); //The behaviour to execute when the mouse is clicked on this UI element.
} UIComponent;

//MOVEMENT COMPONENT BEHAVIOUR: should move the entity
typedef struct
{
    vec2 velocity;
    float speed;
} MovementComponent;

//PHYSICS COMPONENT BEHAVIOUR: should check if the entity is colliding with another entity if it has the same component, and if that's so, trigger the collision on both
typedef struct
{
    CollisionLayer layersBitmask; //with who can this component collide?
    CollisionLayer selfLayer; //which bit do this component occupy?
    float colliderRadius;  //the collision radius of this component
    float collisionsBlockTime; //the time that must pass before this component will be able to collide again
    float collisionsBlockTimeElapsed; //the time passed since the last collision
    void (*Collide)(struct Component *selfComponent, struct Component *otherComponent, struct Game* game);
    boolean canCollide;    //has this collider collided in this frame?
} PhysicsComponent;

//HEALTH COMPONENT BEHAVIOUR: should manage health changes and check if the current health of the entity is <= 0 to trigger death if it happens
typedef struct
{
    int maxLives;
    int currentLives;
    float currentHealth;
    float maxHealth;
    void (*ChangeHealth)(void *selfComponent, float amount);
    void (*Die)(struct Component* selfComponent, struct Game* game);
} HealthComponent;

//ANIMATOR COMPONENT BEHAVIOUR: should set and update a current animation to change the sprite rendered by the RenderComponent
typedef struct
{
    double frameDuration;
    double timeElapsed;
    struct GFXEngine* engine;
    struct SpriteSheet currentAnimation;
    int currentFrameIndex;
    void (*SetAnimation)(struct Component *selfComponent, AnimationType type, float frameDuration);
} AnimatorComponent;

//RENDER COMPONENT BEHAVIOUR: should render the entity sprite
typedef struct
{
    struct Sprite sprite;
    SDL_Renderer* renderer;
} RenderComponent;

//-----END OF ECS SYSTEMS-----//

//-----DATA SYSTEMS-----//
/*All the components of this category have a negative index in the ComponentType enum, and are never updated by the ECS.
They don't need to have a behaviour, but if you want to use it for any reason, remember to fill it or they will fire a nullptr*/

typedef struct
{
    vec2 position;
} TransformComponent;

typedef struct
{
    struct AudioEngine* engine;
    struct Audio audio;
    int loops;
    void (*SetAudio)(struct Component* selfComponent, AudioType type, int loops);
    void (*PlayAudio)(struct Component *selfComponent, struct Game *game);
    boolean isPlaying;
} AudioComponent;

typedef struct
{
    FontType fontType;
    int size;
    char* text;
} TextComponent;

//-----END OF DATA SYSTEMS-----//

struct EntityComponentSystem *ECSInit();
void DestroyECS(struct EntityComponentSystem *ECS);
struct EntityComponentSystem* ECSReset(struct EntityComponentSystem* ECS);
struct Entity *CreateEntity(EntityType type, EntityFamily family, boolean activateImmediately, struct EntityComponentSystem *ECS);
struct Component *AddComponent(struct Entity *entity, ComponentType type, void (*behaviour)(struct Component *selfComponent, struct Game* game));
struct Component *GetComponent(struct Entity *entity, ComponentType type);
void *GetComponentData(struct Entity *entity, ComponentType type);
void MarkComponentToDestroy(struct Component *component);
void MarkEntityToDestroy(struct Entity *entity);
void DestroyComponent(struct Component *component);
void DestroyComponents(aiv_vector *componentsContainer);
void DestroyEntity(struct Entity *entity);
void DestroyEntities(aiv_vector *entitiesContainer);
void AddToSystems(struct Component *component);
void RemoveFromSystems(struct Component *component);
void RegisterEntity(struct Entity *entity);
void UnregisterEntity(struct Entity *entity);
void SetEntityActiveStatus(struct Entity* entity, boolean active);
void SetComponentActiveStatus(struct Component* component, boolean active);

#endif //ECS_H