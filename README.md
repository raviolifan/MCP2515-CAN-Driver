# MCP2515 CAN Driver

A register-level CAN driver for the **MCP2515 CAN Controller** using an **ESP32-S3** and the Arduino framework.

This project implements the MCP2515 from the datasheet without relying on third-party CAN libraries. The driver provides low-level SPI communication, controller initialization, CAN message transmission, CAN message reception, loopback testing, and two-node communication support.

---

## Features

* Register-level MCP2515 driver
* SPI communication interface
* MCP2515 initialization and reset
* CAN controller mode selection
* 500 kbps CAN configuration (8 MHz oscillator)
* Register read/write operations
* Bit Modify command support
* Sequential register read/write support
* CAN message transmission
* CAN message reception
* Loopback mode testing
* Two-node CAN communication support
* Modular driver architecture

---

## Hardware

* ESP32-S3 Development Board
* MCP2515 CAN Controller
* TJA1050 CAN Transceiver
* Two MCP2515 modules (for two-node communication)

---

## Wiring

| MCP2515   | ESP32-S3 |
| --------- | -------- |
| VCC       | 3.3V     |
| GND       | GND      |
| INT       | GPIO 7   |
| SCK       | GPIO 18  |
| SI (MOSI) | GPIO 17  |
| SO (MISO) | GPIO 16  |
| CS        | GPIO 15  |

For two-node communication:

| MCP2515 #1 | MCP2515 #2 |
| ---------- | ---------- |
| CANH       | CANH       |
| CANL       | CANL       |
| GND        | GND        |


---

## Public API

```cpp
bool canInit();

bool canSendMessage(const CanMessage *message);

bool canReceiveMessage(CanMessage *message);

bool canMessageAvailable();
```

The application interacts only with the public API while the driver manages SPI communication, register access, and CAN controller configuration internally.

---

## Loopback Example

```cpp
CanMessage tx;

tx.id = 0x123;
tx.length = 8;

for (int i = 0; i < 8; i++)
{
    tx.data[i] = i;
}

canSendMessage(&tx);

CanMessage rx;

if (canReceiveMessage(&rx))
{
    Serial.println("PASS");
}
```

Loopback mode allows the MCP2515 to internally receive its own transmitted message, making it possible to verify the driver without a second CAN node.


---

## License

This project is released under the MIT License.
