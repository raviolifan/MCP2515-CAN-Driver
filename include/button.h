#ifndef BUTTON_H
#define BUTTON_H

void buttonInit();
void buttonUpdate();        // Must be called once every iteration of loop().
bool buttonPressed();
bool buttonReleased();
bool buttonHeld();
bool buttonLongPress();

#endif