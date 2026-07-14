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
#include "can_internal.h"
#include "can_registers.h"
#include "can_commands.h"
#include "spimodule.h"

///////////////////////////////////////////
// Select MCP2515
//
// Pulls the chip select line low to begin
// an SPI transaction.
///////////////////////////////////////////
void canSelect()
{
    digitalWrite(CAN_CS_PIN, LOW);
}

///////////////////////////////////////////
// Deselect MCP2515
//
// Pulls the chip select line high to end
// an SPI transaction.
///////////////////////////////////////////
void canDeselect()
{
    digitalWrite(CAN_CS_PIN, HIGH);
}

///////////////////////////////////////////
// SPI Transfer
//
// Sends one byte over MOSI while
// simultaneously receiving one byte
// over MISO.
///////////////////////////////////////////
uint8_t canTransfer(uint8_t data)
{
    return SPI.transfer(data);
}

///////////////////////////////////////////
// Reset MCP2515
//
// Sends the reset command to restore the
// controller to its default state.
///////////////////////////////////////////
void canReset()
{
    canSelect();

    canTransfer(CAN_MCP_RESET);

    canDeselect();

    delay(10);
}

///////////////////////////////////////////
// Configure Bit Timing
//
// Configures the MCP2515 for
// 500 kbps using an 8 MHz oscillator.
///////////////////////////////////////////
void canSetBitTiming()
{
    canWriteRegister(CNF1, 0x00);
    canWriteRegister(CNF2, 0x90);
    canWriteRegister(CNF3, 0x02);
}

///////////////////////////////////////////
// Read Register
//
// Reads a single register from the MCP2515.
//
// Parameters:
//   address - Register address
//
// Returns:
//   Register value
///////////////////////////////////////////
uint8_t canReadRegister(uint8_t address)
{
    uint8_t value;

    canSelect();

    canTransfer(CAN_MCP_READ);              // Send read command
    canTransfer(address);               // Send register address

    // Send a dummy byte to generate the
    // clock pulses needed for the MCP2515
    // to return the register value.
    value = canTransfer(0x00);

    canDeselect();

    return value;
}

///////////////////////////////////////////
// Write Register
//
// Writes a single byte to an MCP2515
// register.
//
// Parameters:
//   address - Register address
//   value   - Data to write
///////////////////////////////////////////
void canWriteRegister(uint8_t address, uint8_t value)
{
    canSelect();

    canTransfer(CAN_MCP_WRITE);             // Send WRITE command
    canTransfer(address);               // Send register address
    canTransfer(value);                 // Send register data

    canDeselect();
}

///////////////////////////////////////////
// Write Multiple Registers
//
// Writes consecutive MCP2515 registers.
//
// Parameters:
//   address - Starting register
//   buffer  - Data buffer
//   length  - Number of bytes
///////////////////////////////////////////
void canWriteRegisters(uint8_t address,
                       const uint8_t *buffer,
                       uint8_t length)
{
    canSelect();

    canTransfer(CAN_MCP_WRITE);

    canTransfer(address);

    while (length--)
    {
        canTransfer(*buffer++);
    }

    canDeselect();
}

///////////////////////////////////////////
// Read Multiple Registers
//
// Reads consecutive MCP2515 registers.
//
// Parameters:
//   address - Starting register
//   buffer  - Destination buffer
//   length  - Number of bytes
///////////////////////////////////////////
void canReadRegisters(uint8_t address,
                      uint8_t *buffer,
                      uint8_t length)
{
    canSelect();

    canTransfer(CAN_MCP_READ);

    canTransfer(address);

    while (length--)
    {
        *buffer++ = canTransfer(0x00);
    }

    canDeselect();
}

///////////////////////////////////////////
// Bit Modify
//
// Modifies selected bits within a register.
//
// Parameters:
//   address - Register address
//   mask    - Bits to modify
//   data    - New bit values
///////////////////////////////////////////
void canBitModify(uint8_t address,
                  uint8_t mask,
                  uint8_t data)
{
    canSelect();

    canTransfer(CAN_MCP_BIT_MODIFY);
    canTransfer(address);
    canTransfer(mask);
    canTransfer(data);

    canDeselect();
}

///////////////////////////////////////////
// Read Status
//
// Reads the MCP2215 status Register
// Returns: Controller status byte
///////////////////////////////////////////
uint8_t canReadStatus()
{
    uint8_t status;

    canSelect();
    
    canTransfer(CAN_MCP_READ_STATUS);

    status = canTransfer(0x00);

    canDeselect();

    return status;
}

///////////////////////////////////////////
// Set Operating Mode
//
// Changes the MCP2515 operating mode and
// verifies that the controller entered the
// requested mode.
//
// Parameters:
//   mode - Desired operating mode
//
// Returns:
//   true  - Mode successfully changed
//   false - Mode change failed
///////////////////////////////////////////
bool canSetMode(uint8_t mode)
{
    // Update only the REQOP bits.
    canBitModify(CANCTRL,
                 CANCTRL_REQOP,
                 mode);

    for (int i = 0; i < 10; i++)
    {
        uint8_t status = canReadRegister(CANSTAT);

        if ((status & CANCTRL_REQOP) == mode)
        {
            return true;
        }
        delay(1);
    }

    return false;
}

///////////////////////////////////////////
// Configure Receive Filters
//
// Configures the receive masks and
// acceptance filters to receive all
// standard CAN messages.
///////////////////////////////////////////
void canConfigureFilters()
{
    // Accept all IDs
    canWriteRegister(RXM0SIDH, 0x00);
    canWriteRegister(RXM0SIDL, 0x00);

    canWriteRegister(RXF0SIDH, 0x00);
    canWriteRegister(RXF0SIDL, 0x00);

    canWriteRegister(RXB0CTRL, 0x60);
}

///////////////////////////////////////////
// Transmit Buffer Busy
//
// Checks whether transmit buffer 0 is
// currently transmitting a CAN message.
//
// Returns:
//   true  - Buffer busy
//   false - Buffer available
///////////////////////////////////////////
bool canTxBusy()
{
    return (canReadRegister(TXB0CTRL) & TXREQ);
}

///////////////////////////////////////////
// Read Receive Buffer 0
//
// Reads a CAN message from receive buffer
// 0 and converts it into a CanMessage.
//
// Parameters:
//   message - Destination CAN message
///////////////////////////////////////////
void canReadRxBuffer0(CanMessage *message)
{
    uint8_t sidh = canReadRegister(RXB0SIDH);       // Read the two registers that contain
    uint8_t sidl = canReadRegister(RXB0SIDL);       // the 11-bit Standard Identifier (SID).
    
    // The MCP2515 stores the identifier as:
    //
    // SIDH = ID bits 10:3
    // SIDL = ID bits 2:0 in bits 7:5
    //
    // Example:
    //
    // SIDH = abcdefgh
    // SIDL = xyz-----
    //
    // Shift SIDH left by 3 to move it back
    // into bits 10:3.
    //
    // Shift SIDL right by 5 to move bits
    // 7:5 down into bits 2:0.
    //
    // OR the two values together to rebuild
    // the original 11-bit CAN identifier.
    ///////////////////////////////////////////
    // 10 9 8 7 6 5 4 3 | 2 1 0
    // ------------------------+------
    // Stored by MCP2515 as:
    // SIDH
    // 10 9 8 7 6 5 4 3
    // SIDL
    // 2 1 0 - - - - -
    message->id = (sidh << 3) | (sidl >> 5);

    message->length =                               // Lower four bits contain the number
        canReadRegister(RXB0DLC) & 0x0F;            // of data bytes in the message.

    for (uint8_t i = 0; i < message->length; i++)
    {
        message->data[i] =                          // Read each data byte from the receive buffer
            canReadRegister(RXB0D0 + i);            
    }

    // Clear receive interrupt flag
    canBitModify(CANINTF,
                 RX0IF,
                 0x00);
}

///////////////////////////////////////////
// Wait For Transmission
//
// Waits until the MCP2515 finishes
// transmitting the current CAN message.
//
// The TXREQ bit remains set while the
// controller owns the transmit buffer.
// Once transmission completes, TXREQ is
// automatically cleared by the MCP2515.
//
// Returns:
//   true  - Transmission completed
//   false - Timed out waiting
///////////////////////////////////////////
bool canWaitForTransmit()
{
    uint32_t start = millis();

    while(canTxBusy())
    {
        if(millis() - start > 100)
        {
            return false;
        }
    }

    return true;
}

