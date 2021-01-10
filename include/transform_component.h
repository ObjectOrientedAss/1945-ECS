#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#include "ecs.h"
#include "vec2.h"

struct TransformComponent
{
    vec2 position;
};

//INIT
void InitTransformComponent(struct TransformComponent* transformComponent, vec2 position);

#endif //TRANSFORM_COMPONENT_H