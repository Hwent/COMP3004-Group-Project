#include "Button.h"

Button::Button()
{
    this->isPressed = false;    // default state
}

void Button::setButtonState(bool isPressedNew) {this->isPressed = isPressedNew;}

bool Button::getbuttonState() {return this->isPressed;}
