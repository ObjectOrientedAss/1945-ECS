#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H
#include "ecs.h"

//UI SYSTEM COMPONENT
struct UIComponent
{
    boolean isHovering;                                                 //set it to true when Hover Start is called
    void(*OnHoverStart)(struct Component *selfComponent);               //The behaviour to execute when the mouse starts hovering on this UI element.
    void(*OnHoverEnd)(struct Component *selfComponent);                 //The behaviour to execute when the mouse ends hovering on this UI element.
    void(*OnClick)(struct Component *selfComponent, struct Game* game); //The behaviour to execute when the mouse is clicked on this UI element.
};

//INIT
void InitUIComponent(struct Component* buttonComponent, void(*OnClick)(struct Component* selfComponent, struct Game* game), void(*OnHoverStart)(struct Component *selfComponent), void(*OnHoverEnd)(struct Component *selfComponent));

//UPDATE
void UIBehaviour(struct Component *selfComponent, struct Game* game);

//CUSTOM
void ButtonHoverStart(struct Component* selfComponent);
void ButtonHoverEnd(struct Component* selfComponent);
void StartGameClick(struct Component* selfComponent, struct Game* game);
void QuitGameClick(struct Component* selfComponent, struct Game* game);

#endif //UI_COMPONENT_H