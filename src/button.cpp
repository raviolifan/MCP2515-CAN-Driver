///////////////////////////////////////////
// Button Module
//
// Reads the pushbutton input and provides
// edge-detection events (pressed/released).
///////////////////////////////////////////
#include <Arduino.h>

#include "button.h"

static const int BUTTON_PIN = 1;                // GPIO connected to the onboard pushbutton

static bool currentState = false;               // Current debounced button state
static bool previousState = false;              // Previous debounced button state

unsigned long longPressDuration = 500;          // Time (ms) required for a long press

static unsigned long pressStartTime = 0;        // Timestamp when the current press began
static bool longPressReported = false;          // Prevents multiple long-press events

static bool rawState = false;                   // Instantaneous GPIO reading
static bool lastRawState = false;               // Previous raw GPIO reading
static unsigned long debounceStartTime = 0;     // Time the raw input last changed

static const unsigned long DEBOUNCE_TIME = 100;  // Debounce interval (ms)

///////////////////////////////////////////
// Initialize Button Driver
///////////////////////////////////////////
void buttonInit()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);                  // Initialize button

    currentState = (digitalRead(BUTTON_PIN) == LOW);    // Load initial state
    previousState = currentState;                       // Save initial state
}

///////////////////////////////////////////
// Returns true only on the transition
// from released to pressed.
///////////////////////////////////////////
bool buttonPressed()
{
    return (currentState && !previousState);
}

///////////////////////////////////////////
// Returns true only on the transition
// from pressed to released.
///////////////////////////////////////////
bool buttonReleased()
{
    return (!currentState && previousState);
}

///////////////////////////////////////////
// Update Button Driver
//
// Call once every iteration of loop().
//
// Reads the raw button input, applies
// software debouncing, updates the
// debounced button state, and maintains
// long-press timing information.
///////////////////////////////////////////
void buttonUpdate()
{
    previousState = currentState;                       // Save previous debounced state
    lastRawState = rawState;                            // Save previous raw input
    rawState = (digitalRead(BUTTON_PIN) == LOW);        // Read current raw input

    if (rawState != lastRawState)                       // Restart debounce timer whenever the raw input changes
    {
        debounceStartTime = millis();
    }
    if ((rawState == lastRawState) &&                   // Accept the new button state once the input has
        (millis() - debounceStartTime >= DEBOUNCE_TIME))// remained stable for the debounce interval
    {
        if (currentState != rawState)                   // Update the debounced button state only if it has changed
        {
            currentState = rawState;
        }
    }
    if (buttonPressed())                                // Record when a new button press begins
    {
        pressStartTime = millis();
        longPressReported = false;
    }
    if (buttonReleased())                               // Clear long-press status after the button is released
    {
        longPressReported = false;
    }
}

///////////////////////////////////////////
// Returns true while the button remains
// continuously pressed.
///////////////////////////////////////////
bool buttonHeld()
{
    return (currentState && previousState);
}

///////////////////////////////////////////
// Returns true once when the button has
// been held longer than the configured
// long-press duration.
///////////////////////////////////////////
bool buttonLongPress()
{
    if (!buttonHeld())
    {
        return false;
    }
    if (longPressReported)
    {
        return false;
    }
    if (millis() - pressStartTime >= longPressDuration)
    {
        longPressReported = true;
        return true;
    }
    else
    {
        return false;
    }
}
