#ifndef CAN_INTERNAL_H
#define CAN_INTERNAL_H

#include <Arduino.h>

#include "can.h"

void canSelect();
void canDeselect();

uint8_t canTransfer(uint8_t data);

void canReset();


void canSetBitTiming();

uint8_t canReadRegister(uint8_t address);

void canWriteRegister(uint8_t address, uint8_t value);

void canWriteRegisters(uint8_t address,
                       const uint8_t *buffer,
                       uint8_t length);

void canReadRegisters(uint8_t address,
                      uint8_t *buffer,
                      uint8_t length);

void canBitModify(uint8_t address,
                  uint8_t mask,
                  uint8_t data);

uint8_t canReadStatus();
                 
bool canSetMode(uint8_t mode);


void canConfigureFilters();

bool canTxBusy();

void canRequestToSend0();

void canReadRxBuffer0(CanMessage *message);

bool canWaitForTransmit();

#endif