/*
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   version 2 as published by the Free Software Foundation.
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "string.h"

// Hardware configuration
const int pin   = 5;
const char ID[] = "AC01";
bool running    = false;

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);

// Topology
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t receive = 0xACACACACF0LL;
const uint64_t send    = 0xACACACACA3LL;

// Payload
const int payload_size = 32;
// +1 to allow room for a terminating NULL char
char receive_payload[payload_size + 1];
char send_payload[payload_size + 1];

char* getDeviceID(const char* payload)
{
    char* deviceID = (char*)malloc(sizeof(char)*5);
    strncpy(deviceID, &payload[1], 4);
    deviceID[4] = '\0';
    return deviceID;
}

void setup(void)
{
    // Setup and configure RF radio
    radio.begin();

    // optionally, increase the delay between retries & # of retries
    radio.setRetries(15, 15);
    radio.setDataRate(RF24_250KBPS);
    radio.setChannel(120);

    // Open pipes to other nodes for communication
    radio.openWritingPipe(send);
    radio.openReadingPipe(1, receive);

    // Start listening
    radio.startListening();

    // Dump the configuration of the rf unit for debugging
    //radio.printDetails();

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

void toggle(void)
{
    digitalWrite(pin, LOW);
    delay(500);
    digitalWrite(pin, HIGH);
}

void loop(void)
{
    // if there is data ready
    if (radio.available())
    {
        // Dump the payloads until we've gotten everything
        bool done = false;
        while (!done)
        {
            // Fetch the payload, and see if this was the last one.
            done = radio.read(receive_payload, payload_size);

            receive_payload[payload_size] = '\0';
            strcpy(send_payload, receive_payload);

            if (receive_payload[0] == 'S' && strcmp(getDeviceID(receive_payload), ID) == 0)
            {
                send_payload[5] = '$';
                switch (receive_payload[5])
                {
                    case 'S':
                        if (running)
                            send_payload[5] = 'O';
                        else
                            send_payload[5] = 'F';
                        break;

                    case 'O':
                        if (!running)
                            toggle();

                        running = true;
                        send_payload[5] = 'O';
                        break;

                    case 'F':
                        if (running)
                            toggle();

                        running = false;
                        send_payload[5] = 'F';
                        break;
                }

                send_payload[0] = 'R';

                // First, stop listening so we can talk
                radio.stopListening();

                // Send the final one back.
                radio.write(send_payload, payload_size);

                // Now, resume listening so we catch the next packets.
                radio.startListening();
            }
        }
    }
}
