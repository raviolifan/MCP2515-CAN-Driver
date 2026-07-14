///////////////////////////////////////////
// CAN Transmitter (Master)
//
// Waits for a button press to send a
// PING message over the CAN bus. After
// transmitting, waits for a PONG response
// from the receiver before allowing
// another PING.
///////////////////////////////////////////
#include <Arduino.h>

#include "can.h"

#include "can_internal.h"
#include "can_registers.h"
#include "can_commands.h"
#include "button.h"

///////////////////////////////////////////
// Global CAN Message
//
// Message buffer used for receiving
// incoming CAN frames.
///////////////////////////////////////////
CanMessage message;

///////////////////////////////////////////
// Send PING
//
// Builds and transmits a PING message to
// the receiver.
///////////////////////////////////////////
void sendPing()
{
    CanMessage message;

    message.id = CAN_ID_PING;
    message.length = 1;

    message.data[0] = CAN_CMD_PING;

    canSendMessage(&message);
}

///////////////////////////////////////////
// Setup
//
// Initializes serial communication,
// button driver, and CAN controller.
///////////////////////////////////////////
void setup()
{
    Serial.begin(115200);

    buttonInit();
    canInit();
    uint8_t mode = canReadRegister(CANSTAT);

    Serial.print("CANSTAT = 0x");
    Serial.println(mode, HEX);
    Serial.println("CAN Transmitter Ready");
}

///////////////////////////////////////////
// Main Loop
//
// Waits for a button press. When pressed,
// transmits a PING message and waits for
// a PONG response before allowing another
// transmission.
///////////////////////////////////////////
void loop()
{
    static bool waitingForPong = false;

    if (!waitingForPong)
    {
        buttonUpdate();                         // Update button state machine

        if (buttonPressed())                    // Send ping on rising edge of the button press
        {
            sendPing();

            waitingForPong = true;

            Serial.println("PING SENT");
        }
    }

    CanMessage message;

    if (canReceiveMessage(&message))            // Check for incoming CAN messages
    {
        if (message.id == CAN_ID_PONG)          // Process only PONG responses
        {
            Serial.println("PONG RECEIVED");    

            waitingForPong = false;             // Allow the next button press to send another PING
        }
    }
}