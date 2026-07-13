#ifndef CAN_REGISTERS_H
#define CAN_REGISTERS_H

#include <Arduino.h>

///////////////////////////////////////////
// MCP2515 Register Addresses
///////////////////////////////////////////

// Control and Status
constexpr uint8_t CANSTAT  = 0x0E;
constexpr uint8_t CANCTRL  = 0x0F;

// Bit Timing Configuration
constexpr uint8_t CNF3     = 0x28;
constexpr uint8_t CNF2     = 0x29;
constexpr uint8_t CNF1     = 0x2A;

// const uint8_t timing[] =
// {
//     0x02,   // CNF3
//     0x90,   // CNF2
//     0x00    // CNF1
// };

// Interrupts
constexpr uint8_t CANINTE  = 0x2B;
constexpr uint8_t CANINTF  = 0x2C;
constexpr uint8_t EFLG     = 0x2D;

// Operating Modes
constexpr uint8_t MODE_NORMAL        = 0x00;
constexpr uint8_t MODE_SLEEP         = 0x20;
constexpr uint8_t MODE_LOOPBACK      = 0x40;
constexpr uint8_t MODE_LISTEN_ONLY   = 0x60;
constexpr uint8_t MODE_CONFIGURATION = 0x80;

constexpr uint8_t CANCTRL_REQOP = 0xE0;

///////////////////////////////////////////
// Transmit Buffer 0 Registers
///////////////////////////////////////////
constexpr uint8_t TXB0CTRL  = 0x30;

constexpr uint8_t TXB0SIDH  = 0x31;
constexpr uint8_t TXB0SIDL  = 0x32;

constexpr uint8_t TXB0DLC   = 0x35;

constexpr uint8_t TXB0D0    = 0x36;

///////////////////////////////////////////
// Receive Buffer 0
///////////////////////////////////////////

constexpr uint8_t RXB0CTRL = 0x60;

constexpr uint8_t RXB0SIDH = 0x61;
constexpr uint8_t RXB0SIDL = 0x62;

constexpr uint8_t RXB0DLC = 0x65;

constexpr uint8_t RXB0D0 = 0x66;

constexpr uint8_t RX0IF = 0x01;         // Receieve flag

///////////////////////////////////////////
// Acceptance Filters
///////////////////////////////////////////

constexpr uint8_t RXF0SIDH = 0x00;
constexpr uint8_t RXF0SIDL = 0x01;

///////////////////////////////////////////
// Acceptance Masks
///////////////////////////////////////////

constexpr uint8_t RXM0SIDH = 0x20;
constexpr uint8_t RXM0SIDL = 0x21;

constexpr uint8_t TXREQ = 0x08;


#endif