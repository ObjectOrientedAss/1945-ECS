#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H
#include "ecs.h"

//INPUT SYSTEM COMPONENT
struct InputComponent
{
    float xAxis; //the x axis movement value registered from SDL
    float yAxis; //the y axis movement value registered from SDL
    boolean shoot; //the shooting input value registered from SDL
};

//UPDATE
void InputBehaviour(struct Component *selfComponent, struct Game* game);

#endif //INPUT_COMPONENT_H