///////////////////////////////////////////
// CAN Driver
//
// Driver for the MCP2515 CAN controller.
// Implements initialization, register
// access, message transmission, and
// message reception over the CAN bus.
///////////////////////////////////////////
#include <Arduino.h>
#include <SPI.h>
#include <cstring>

#include "can.h"
#include "can_registers.h"
#include "can_internal.h"
#include "can_commands.h"
#include "spimodule.h"

///////////////////////////////////////////
// Initialize CAN Driver
//
// Initializes the SPI bus, configures the
// MCP2515 control pins, and resets the
// controller.
///////////////////////////////////////////
void canInit()
{
    spiInit();

    pinMode(CAN_CS_PIN, OUTPUT);
    digitalWrite(CAN_CS_PIN, HIGH);

    pinMode(CAN_INT_PIN, INPUT_PULLUP);

    canReset();

    canSetBitTiming();

    canConfigureFilters();

    canSetMode(MODE_LOOPBACK);

    CanMessage tx;

    tx.id = 0x123;

    tx.length = 8;

    for(int i=0;i<8;i++)
    {
        tx.data[i]=i;
    }

    canSendMessage(&tx);

    CanMessage rx;

    if(canReceiveMessage(&rx))
    {
        Serial.println("PASS");
    }
    else
    {
        Serial.println("FAIL");
    }
}


///////////////////////////////////////////
// Request To Send
//
// Starts transmission of TX Buffer 0.
///////////////////////////////////////////
void canRequestToSend0()
{
    canSelect();

    canTransfer(MCP_RTS_TX0);

    canDeselect();
}



///////////////////////////////////////////
// Message Available
//
// Returns true if RX Buffer 0 contains
// a received CAN message.
///////////////////////////////////////////
bool canMessageAvailable()
{
    return (canReadRegister(CANINTF) & RX0IF);
}


///////////////////////////////////////////
// Build Transmit Buffer
//
// Converts a CAN message into the MCP2515
// transmit buffer format.
//
// Parameters:
//   message - CAN message to transmit
//   buffer  - Destination transmit buffer
///////////////////////////////////////////
static void canBuildTxBuffer(
    const CanMessage *message,
    uint8_t *buffer)
{
    buffer[0] = message->id >> 3;

    buffer[1] = (message->id & 0x07) << 5;

    buffer[2] = 0;

    buffer[3] = 0;

    buffer[4] = message->length & 0x0F;

    memcpy(&buffer[5],
           message->data,
           message->length);
}

///////////////////////////////////////////
// Load Transmit Buffer 0
//
// Copies a formatted transmit buffer into
// MCP2515 transmit buffer 0 using the
// Load TX Buffer SPI command.
//
// Parameters:
//   buffer - Transmit buffer data
//   length - Number of bytes to transfer
///////////////////////////////////////////
static void canLoadTxBuffer0(
    const uint8_t *buffer,
    uint8_t length)
{
    canSelect();

    canTransfer(MCP_LOAD_TX);

    for(uint8_t i = 0; i < length; i++)
    {
        canTransfer(buffer[i]);
    }

    canDeselect();
}


///////////////////////////////////////////
// Send Message
//
// Sends a CAN message using transmit
// buffer 0.
//
// Returns:
//     true  Message queued
//     false Buffer busy
///////////////////////////////////////////
bool canSendMessage(const CanMessage *message)
{
    if (canTxBusy())
    {
        return false;
    }

    uint8_t buffer[13];

    canBuildTxBuffer(message, buffer);

    canLoadTxBuffer0(buffer,
                     5 + message->length);

    canRequestToSend0();

    return canWaitForTransmit();
}

bool canReceiveMessage(CanMessage *message)
{
    if(!canMessageAvailable())
    {
        return false;
    }

    canReadRxBuffer0(message);

    return true;
}