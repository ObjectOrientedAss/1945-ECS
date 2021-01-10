#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H
#include "ecs.h"

struct TextComponent
{
    FontType fontType;
    int size;
    char* text;
};

//INIT
void InitTextComponent(struct TextComponent* textComponent, char* startingText, FontType fontType, int size);

#endif //TEXT_COMPONENT_H