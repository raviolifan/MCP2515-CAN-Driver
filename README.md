# MCP2515 CAN Driver

A register-level CAN driver for the **MCP2515 CAN Controller** using an **ESP32-S3** and the Arduino framework.

This project implements the MCP2515 directly from the datasheet without relying on third-party CAN libraries. The driver provides low-level SPI communication, controller initialization, CAN message transmission, CAN message reception, loopback testing, and two-node CAN communication using a custom Ping/Pong protocol.

---

## Features

- Register-level MCP2515 driver
- SPI communication interface
- MCP2515 initialization and reset
- CAN controller mode selection
- 500 kbps CAN configuration (8 MHz oscillator)
- Register read/write operations
- Bit Modify command support
- Sequential register read/write support
- Standard 11-bit CAN frame support
- CAN message transmission
- CAN message reception
- Loopback mode testing
- Two-node CAN communication
- Ping/Pong request-response protocol
- Button-triggered message transmission
- Modular driver architecture

---

## Hardware

- 2 × ESP32-S3 Development Boards
- 2 × MCP2515 CAN Controller Modules
- 2 × TJA1050 CAN Transceivers
- Push Button
- 120 Ω CAN Bus Termination (one at each end)

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

## Loopback Test

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

Loopback mode internally routes transmitted messages back into the receive buffer, allowing the driver to be verified without a second CAN node.

---

## Two-Node Ping/Pong Demo

The project also demonstrates communication between two ESP32-S3 nodes connected through a CAN bus.

### Node A (Master)

- Waits for a button press
- Sends a **PING** message
- Waits for a **PONG** response

### Node B (Slave)

- Waits for incoming CAN messages
- Detects a **PING**
- Responds with a **PONG**

```
Button
   │
   ▼

ESP32 #1
(PING)
   │
   ▼
=====================
      CAN BUS
=====================
   ▲
   │
ESP32 #2
(PONG)
   │
   ▼
ESP32 #1
```

Example output:

**Transmitter**

```text
CAN Transmitter Ready

PING SENT

PONG RECEIVED
```

**Receiver**

```text
CAN Receiver Ready

PING

PONG SENT
```

---

## License

This project is released under the MIT License.
