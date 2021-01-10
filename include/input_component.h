#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H
#include "ecs.h"

//INPUT COMPONENT BEHAVIOUR: should register the inputs from an external device
struct InputComponent
{
    float xAxis;
    float yAxis;
    boolean shoot;
};

//UPDATE
void InputBehaviour(struct Component *selfComponent, struct Game* game);

#endif //INPUT_COMPONENT_H