#include "transform_component.h"

void InitTransformComponent(struct TransformComponent *transformComponent, vec2 position)
{
    transformComponent->position = position;
    //printf("\n---Transform Component Initialized!");
}