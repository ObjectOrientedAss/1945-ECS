#include "text_component.h"

void InitTextComponent(struct TextComponent *textComponent, char *startingText, FontType fontType, int size)
{
    textComponent->fontType = fontType;
    textComponent->size = size;
    textComponent->text = startingText;
}