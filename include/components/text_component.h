#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H
#include "ecs.h"

//TEXT DATA COMPONENT
struct TextComponent
{
    FontType fontType;  //the font type to be used to display text
    int size;   //the characters size of the text in points (not pixels)
    char* text;     //the actual text to be displayed
};

//INIT
void InitTextComponent(struct TextComponent* textComponent, char* startingText, FontType fontType, int size);

#endif //TEXT_COMPONENT_H