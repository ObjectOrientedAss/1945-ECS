#include "input_component.h"

void InputBehaviour(struct Component *selfComponent, struct Game *game)
{
    //printf("Input Behaviour Called\n");
    struct InputComponent *inputComponent = selfComponent->data;
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    //inputComponent->shoot = false;

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