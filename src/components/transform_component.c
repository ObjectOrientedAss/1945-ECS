#include "transform_component.h"

void InitTransformComponent(struct TransformComponent *transformComponent, vec2 position, vec2 scale)
{
    transformComponent->position = position;
    transformComponent->scale = scale;
    if (scale.x <= 0)
        scale.x = 1;
    if (scale.y <= 0)
        scale.y = 1;
}

void SetPosition(struct TransformComponent* transformComponent, vec2 position)
{
    transformComponent->position = position;
}