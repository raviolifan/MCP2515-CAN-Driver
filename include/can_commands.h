#ifndef CAN_COMMANDS_H
#define CAN_COMMANDS_H

#include <Arduino.h>

///////////////////////////////////////////
// Pin Assignments
///////////////////////////////////////////
constexpr uint8_t CAN_CS_PIN  = 15;
constexpr uint8_t CAN_INT_PIN = 7;

///////////////////////////////////////////
// MCP2515 SPI Commands
///////////////////////////////////////////
constexpr uint8_t MCP_RESET         = 0xC0;
constexpr uint8_t MCP_READ          = 0x03;
constexpr uint8_t MCP_READ_RX       = 0x90;
constexpr uint8_t MCP_WRITE         = 0x02;
constexpr uint8_t MCP_LOAD_TX       = 0x40;
constexpr uint8_t MCP_RTS           = 0x80;
constexpr uint8_t MCP_READ_STATUS   = 0xA0;
constexpr uint8_t MCP_RX_STATUS     = 0xB0;
constexpr uint8_t MCP_BIT_MODIFY    = 0x05;

constexpr uint8_t MCP_RTS_TX0 = 0x81;           // Request transmission


#endif