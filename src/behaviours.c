#include "behaviours.h"
#include "stdio.h"

void InputBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Input Behaviour Called\n");
    InputComponent *inputComponent = (InputComponent *)selfComponent->data;
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    //horizontal input
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
        inputComponent->xAxis = -1.f;
    else if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
        inputComponent->xAxis = 1.f;
    else
        inputComponent->xAxis = 0;

    //vertical input
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
        inputComponent->yAxis = -1.f;
    else if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
        inputComponent->yAxis = 1.f;
    else
        inputComponent->yAxis = 0;

    //shoot input
    if (state[SDL_SCANCODE_SPACE])
        inputComponent->shoot = true;
    else
        inputComponent->shoot = false;
}

void ButtonBehaviour(struct Component *selfComponent, struct Game *game)
{
    ButtonComponent *buttonComponent = (ButtonComponent *)selfComponent->data;

    RenderComponent *renderComponent = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);
    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);

    //if the mouse is over the button
    if (game->mousePositionX >= tc->position.x - (renderComponent->sprite.spriteRect.w * 0.5f) &&
        game->mousePositionX <= tc->position.x + (renderComponent->sprite.spriteRect.w * 0.5f) &&
        game->mousePositionY >= tc->position.y - (renderComponent->sprite.spriteRect.h * 0.5f) &&
        game->mousePositionY <= tc->position.y + (renderComponent->sprite.spriteRect.h * 0.5f))
    {
        //if it is the first frame in which the mouse has started hovering
        if (!buttonComponent->isHovering)
        {
            if (buttonComponent->OnHoverStart != NULL)
                buttonComponent->OnHoverStart(selfComponent);
        }
        else
        {
            if (game->mouseDown)
                buttonComponent->OnClick(selfComponent, game);
        }
    }
    else //if the mouse is not over the button
    {
        if (buttonComponent->isHovering)
        {
            if (buttonComponent->OnHoverEnd != NULL)
                buttonComponent->OnHoverEnd(selfComponent);
        }
    }
}

void PlayerMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("PlayerMovement Behaviour Called\n");
    MovementComponent *movementComponent = (MovementComponent *)selfComponent->data;
    InputComponent *ic = (InputComponent *)GetComponentData(selfComponent->owner, InputC);
    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);

    movementComponent->velocity.x = ic->xAxis;
    movementComponent->velocity.y = ic->yAxis;

    tc->position.x += movementComponent->velocity.x * movementComponent->speed * game->engine->time->deltaTime;
    tc->position.y += movementComponent->velocity.y * movementComponent->speed * game->engine->time->deltaTime;
}

void EnemyMovementBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Enemy Behaviour Called\n");
    MovementComponent *movementComponent = (MovementComponent *)selfComponent->data;
    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);

    tc->position.x += movementComponent->velocity.x * movementComponent->speed * game->engine->time->deltaTime;
    tc->position.y += movementComponent->velocity.y * movementComponent->speed * game->engine->time->deltaTime;
}

void PhysicsBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Physics Behaviour Called\n");
    //get my physics component
    PhysicsComponent *physicsComponent = (PhysicsComponent *)selfComponent->data;

    if (physicsComponent->hasCollided) //if i have already collided in this frame, don't check anything!
        return;

    //get my movement component
    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);

    //iterate all the other physics components!
    aiv_vector *physicsSystem = (aiv_vector *)aiv_vector_at(selfComponent->owner->ECS->systems, (int)selfComponent->type);
    for (uint i = 0; i < aiv_vector_size(physicsSystem); i++)
    {
        struct Component *otherComponent = (struct Component *)aiv_vector_at(physicsSystem, i);

        //get the other physics component
        PhysicsComponent *otherPhysicsComponent = (PhysicsComponent *)otherComponent->data;
        if (otherPhysicsComponent->hasCollided) //the other collider has already collided in this frame!
            continue;
        if (physicsComponent == otherPhysicsComponent) //i am also in the same list, so, if this is myself...
            continue;

        //check if the collision layers are compatible
        if ((physicsComponent->collisionBitmask & otherPhysicsComponent->componentBitmask) != 0)
        {
            //get the other movement component
            MovementComponent *otherMovementComponent = (MovementComponent *)GetComponentData(otherComponent->owner, MovementC);
            //get the distance vector between me and the other
            vec2 vDist = vec2_sub(&tc->position, &tc->position);
            //if the distance is less than the two colliders radiuses combined, we are colliding on each other
            if (vec2_magn(&vDist) <= physicsComponent->colliderRadius + otherPhysicsComponent->colliderRadius)
            {
                //handle the collision on me:
                physicsComponent->hasCollided = true;
                physicsComponent->Collide(selfComponent, otherComponent);
                //trigger and handle the collision on the other:
                otherPhysicsComponent->hasCollided = true;
                otherPhysicsComponent->Collide(otherComponent, selfComponent);
            }
        }

        break; //i just want to be able to collide once per frame, so after colliding, stop checking collisions on this object!
    }
}

void HealthBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Health Behaviour Called\n");
    //get my health component
    HealthComponent *healthComponent = (HealthComponent *)selfComponent->data;
    if (healthComponent->currentHealth <= 0)
    {
        //die bitch
        // Component *otherComponent = (Component *)GetComponent(selfComponent->owner, AnimatorC);
        // AnimatorComponent *animatorComponent = otherComponent->data;
        // if (selfComponent->owner->type == Player)
        //     animatorComponent->SetAnimation(otherComponent, PlayerExplosionA);
        // else if (selfComponent->owner->type == Enemy)
        //     animatorComponent->SetAnimation(otherComponent, Enemy1A);
    }
}

void AnimatorBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Animator Behaviour Called\n");
    AnimatorComponent *animatorComponent = (AnimatorComponent *)selfComponent->data;

    if (animatorComponent->currentAnimation.frames <= 1) //if an asshole has created an animation spritesheet with 1 or less frames...
        return;

    RenderComponent *renderComponent = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);

    animatorComponent->timeElapsed += game->engine->time->deltaTime;
    if (animatorComponent->timeElapsed >= animatorComponent->frameDuration)
    {
        animatorComponent->timeElapsed = 0;
        animatorComponent->currentFrameIndex++;
        if (animatorComponent->currentFrameIndex >= animatorComponent->currentAnimation.frames)
            animatorComponent->currentFrameIndex = 0;

        renderComponent->sprite.originRect.x = animatorComponent->currentFrameIndex * animatorComponent->currentAnimation.frameWidth;
    }
}

void RenderBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Render Behaviour Called\n");
    //get my render component
    RenderComponent *renderComponent = (RenderComponent *)selfComponent->data;

    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);
    renderComponent->sprite.spriteRect.x = (int)tc->position.x - (int)(renderComponent->sprite.spriteRect.w * 0.5f);
    renderComponent->sprite.spriteRect.y = (int)tc->position.y - (int)(renderComponent->sprite.spriteRect.h * 0.5f);
    //copy the texture to the rendering context
    //if (renderComponent->sprite.texture == NULL)
    //printf("Texture null");
    SDL_RenderCopy(renderComponent->renderer, renderComponent->sprite.texture, &renderComponent->sprite.originRect, &renderComponent->sprite.spriteRect);
}

//COMPONENTS CALLS FROM OUTSIDE-----------------------------------------------------------------------

void Collide(struct Component *selfComponent, struct Component *otherComponent)
{
    //get my physics component
    PhysicsComponent *selfPhysicsComponent = (PhysicsComponent *)selfComponent->data;
    //get other physics component
    PhysicsComponent *otherPhysicsComponent = (PhysicsComponent *)otherComponent->data;

    switch (selfComponent->owner->type)
    {
    case Player:
        if (otherComponent->type == Enemy)
        {
            HealthComponent *healthComponent = (HealthComponent *)GetComponentData(selfComponent->owner, HealthC);
            DamageComponent *damageComponent = (DamageComponent *)GetComponentData(otherComponent->owner, DamageC);
            healthComponent->ChangeHealth(healthComponent, damageComponent->damage);
        }
        else if (otherComponent->type == EnemyBullet)
        {
            HealthComponent *healthComponent = (HealthComponent *)GetComponentData(selfComponent->owner, HealthC);
            DamageComponent *damageComponent = (DamageComponent *)GetComponentData(otherComponent->owner, DamageC);
            healthComponent->ChangeHealth(healthComponent, damageComponent->damage);
        }
        break;
    case Enemy:
        if (otherComponent->type == Player)
        {
            HealthComponent *healthComponent = (HealthComponent *)GetComponentData(selfComponent->owner, HealthC);
            healthComponent->ChangeHealth(healthComponent, healthComponent->maxHealth);
        }
        else if (otherComponent->type == PlayerBullet)
        {
            HealthComponent *healthComponent = (HealthComponent *)GetComponentData(selfComponent->owner, HealthC);
            DamageComponent *damageComponent = (DamageComponent *)GetComponentData(otherComponent->owner, DamageC);
            healthComponent->ChangeHealth(healthComponent, damageComponent->damage);
        }
        break;
    case PlayerBullet:
        break;
    case EnemyBullet:
        break;
    }
}

void ChangeHealth(HealthComponent *selfComponent, float amount)
{
    selfComponent->currentHealth += amount;
    if (selfComponent->currentHealth > selfComponent->maxHealth)
        selfComponent->currentHealth = selfComponent->maxHealth;
}

void SetAnimation(struct Component *selfComponent, AnimationType type, float frameDuration)
{
    AnimatorComponent *animatorComponent = (AnimatorComponent *)selfComponent->data;

    RenderComponent *renderComponent = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);

    animatorComponent->currentAnimation = GetAnimation(animatorComponent->engine, type);
    animatorComponent->frameDuration = animatorComponent->currentAnimation.baseFrameDuration;
    animatorComponent->timeElapsed = 0;
    animatorComponent->currentFrameIndex = 0;
    animatorComponent->frameDuration = frameDuration;

    renderComponent->sprite.texture = animatorComponent->currentAnimation.texture;
    renderComponent->sprite.originRect.x = animatorComponent->currentFrameIndex * animatorComponent->currentAnimation.frameWidth;
    renderComponent->sprite.spriteRect.w = animatorComponent->currentAnimation.frameWidth;
    renderComponent->sprite.spriteRect.h = animatorComponent->currentAnimation.frameHeight;
}

//in this game i want all the buttons to have the same hover start and hover end behaviours so i can recycle them...

void ButtonHoverStart(struct Component *selfComponent)
{
    printf("Hovering on button started");
}

void ButtonHoverEnd(struct Component *selfComponent)
{
    printf("Hovering on button ended");
}

//the start button and the quit button

void StartGameClick(struct Component *selfComponent, struct Game *game)
{
    printf("Clicking on Start Game");
    game->sceneToLoad = GameScene;
}

void QuitGameClick(struct Component *selfComponent, struct Game *game)
{
    printf("Clicking on Quit Game");
    game->quitRequest = true;
}

//COMPONENTS INIT METHODS----------------------------------------------------------------------------------------

void InitMovementComponent(MovementComponent *movementComponent, vec2 velocity, float speed)
{
    movementComponent->velocity = velocity;
    movementComponent->speed = speed;
    printf("\n---Movement Component Initialized!");
}

void InitTransformComponent(TransformComponent *transformComponent, vec2 position)
{
    transformComponent->position = position;
    printf("\n---Transform Component Initialized!");
}

void InitButtonComponent(struct Component *selfComponent, void (*OnClick)(struct Component *selfComponent, struct Game *game), vec2 position)
{
    ButtonComponent *buttonComponent = (ButtonComponent *)selfComponent->data;
    TransformComponent *tc = (TransformComponent *)GetComponentData(selfComponent->owner, TransformC);
    buttonComponent->OnClick = OnClick;
    buttonComponent->OnHoverEnd = NULL;
    buttonComponent->OnHoverStart = NULL;
    tc->position = position;
    printf("\n---Button Component Initialized!");
}

void InitPhysicsComponent(struct Component *selfComponent)
{
    PhysicsComponent *physicsComponent = (PhysicsComponent *)selfComponent->data;
    RenderComponent *renderComponent = (RenderComponent *)GetComponentData(selfComponent->owner, RenderC);
    if (renderComponent == NULL)
        physicsComponent->colliderRadius = 10;
    else
        physicsComponent->colliderRadius = renderComponent->sprite.spriteRect.w * 0.5f;

    //COLLISION MASKS ORDER: EnemyBullet, PlayerBullet, Enemy, Player -> TODO: use the enum to make the bitmask.
    switch (selfComponent->type)
    {
    case Player:
        physicsComponent->collisionBitmask = 1010;
        physicsComponent->componentBitmask = 0001;
        break;
    case Enemy:
        physicsComponent->collisionBitmask = 0101;
        physicsComponent->componentBitmask = 0010;
        break;
    case PlayerBullet:
        physicsComponent->collisionBitmask = 0010;
        physicsComponent->componentBitmask = 0100;
        break;
    case EnemyBullet:
        physicsComponent->collisionBitmask = 0001;
        physicsComponent->componentBitmask = 1000;
        break;
    }
    printf("\n---Physics Component Initialized!");
}

void InitRenderComponent(RenderComponent *renderComponent, struct GFXEngine *engine, SpriteType spriteType)
{
    renderComponent->renderer = engine->renderer;
    renderComponent->sprite = GetSprite(engine, spriteType);
    printf("\n\n-------------------------------------------------\n\n");
    printf("\n\nOR H: %d | OR W: %d | OR X: %d | OR Y: %d", renderComponent->sprite.originRect.h, renderComponent->sprite.originRect.w, renderComponent->sprite.originRect.x, renderComponent->sprite.originRect.y);
    printf("\n\nSR H: %d | SR W: %d | SR X: %d | SR Y: %d", renderComponent->sprite.spriteRect.h, renderComponent->sprite.spriteRect.w, renderComponent->sprite.spriteRect.x, renderComponent->sprite.spriteRect.y);
    printf("\n---Render Component Initialized!");
}

void InitAnimatorComponent(AnimatorComponent *animatorComponent, struct GFXEngine *engine, AnimationType animationType)
{
    animatorComponent->engine = engine;
    animatorComponent->currentAnimation = GetAnimation(engine, animationType);
    printf("AF Width: %d | AF Height: %d\n", animatorComponent->currentAnimation.frameWidth, animatorComponent->currentAnimation.frameHeight);
    printf("\n---Animator Component Initialized!");
}