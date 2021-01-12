#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#include "ecs.h"
#include "vec2.h"

//TRANSFORM DATA COMPONENT
struct TransformComponent
{
    vec2 position;  //the position of the entity in space
    vec2 scale;     //the scale of the entity
};

//INIT
void InitTransformComponent(struct TransformComponent* transformComponent, vec2 position, vec2 scale);

//CUSTOM
void SetPosition(struct TransformComponent* transformComponent, vec2 position);

#endif //TRANSFORM_COMPONENT_H