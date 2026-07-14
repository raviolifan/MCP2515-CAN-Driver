///////////////////////////////////////////
// CAN Receiver (Slave)
//
// Waits for a PING message from the CAN
// network. When a PING is received, the
// receiver immediately responds with a
// PONG message.
///////////////////////////////////////////
#include <Arduino.h>

#include "can.h"

#include "can_internal.h"
#include "can_registers.h"
#include "can_commands.h"

///////////////////////////////////////////
// Global CAN Message
//
// Message buffer used for receiving
// incoming CAN frames.
///////////////////////////////////////////
CanMessage message;

///////////////////////////////////////////
// Send PONG
//
// Builds and transmits a PONG response
// back to the requesting node.
///////////////////////////////////////////
void sendPong()
{
    CanMessage message;

    message.id = CAN_ID_PONG;
    message.length = 1;

    message.data[0] = CAN_CMD_PONG;

    canSendMessage(&message);
}

///////////////////////////////////////////
// Setup
//
// Initializes serial communication and
// the MCP2515 CAN controller.
///////////////////////////////////////////
void setup()
{
    Serial.begin(115200);

    canInit();
    uint8_t mode = canReadRegister(CANSTAT);

    Serial.print("CANSTAT = 0x");
    Serial.println(mode, HEX);
    Serial.println("CAN Receiver Ready");
}

///////////////////////////////////////////
// Main Loop
//
// Waits for incoming CAN messages. If a
// PING message is received, a PONG
// response is transmitted back to the
// sender.
///////////////////////////////////////////
void loop()
{
    CanMessage message;

    if (canReceiveMessage(&message))
    {
        if (message.id == CAN_ID_PING)
        {
            Serial.println("PING");

            sendPong();
        }
    }
}