///////////////////////////////////////////
///////////////////////////////////////////
#include <Arduino.h>

#include "can.h"
#include "can_internal.h"
#include "spimodule.h"

///////////////////////////////////////////
///////////////////////////////////////////
void setup()
{
    Serial.begin(115200);       // Initialize serial communication

    canInit();

    CanMessage tx;

    tx.id = 0x123;
    tx.length = 8;

    for (int i = 0; i < 8; i++)
    {
        tx.data[i] = i;
    }

    if (canSendMessage(&tx))
    {
        Serial.println("TX PASS");
    }
    else
    {
        Serial.println("TX FAIL");
    }

    CanMessage rx;

    if (canReceiveMessage(&rx))
    {
        Serial.println("RX PASS");

        Serial.print("ID: 0x");
        Serial.println(rx.id, HEX);

        for (int i = 0; i < rx.length; i++)
        {
            Serial.print(rx.data[i]);
            Serial.print(' ');
        }

        Serial.println();
    }
    else
    {
        Serial.println("RX FAIL");
    }
    }

///////////////////////////////////////////

///////////////////////////////////////////
void loop()
{

}