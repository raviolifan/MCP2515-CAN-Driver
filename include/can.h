#ifndef CAN_H
#define CAN_H

#include <Arduino.h>

///////////////////////////////////////////
// CAN Message
///////////////////////////////////////////
typedef struct
{
    uint16_t id;       // 11-bit Standard Identifier
    uint8_t length;    // Number of data bytes (0-8)
    uint8_t data[8];   // Payload

    bool extended;
    bool remote;
} CanMessage;


///////////////////////////////////////////
// Public API
///////////////////////////////////////////

void canInit();

bool canSendMessage(const CanMessage *message);

bool canReceiveMessage(CanMessage *message);

bool canMessageAvailable();

#endif