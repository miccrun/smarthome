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
const int pin1  = 4;
const int pin2  = 5;
const char DEVICE_ID[] = "A2";
bool running    = false;

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);

// Topology
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t ac2_receive = 0xACACACAC10LL;
const uint64_t ac2_send    = 0xACACACAC20LL;

// Payload
const int payload_size = 4;
// +1 to allow room for a terminating NULL char
char receive_payload[payload_size + 1];
char send_payload[payload_size + 1];

char* getDeviceID(const char* payload)
{
    char* deviceID = (char*)malloc(sizeof(char)*3);
    strncpy(deviceID, &payload[2], 2);
    deviceID[2] = '\0';
    return deviceID;
}

void setup(void)
{
    // Setup and configure RF radio

    Serial.begin(57600);

    radio.begin();

    // optionally, increase the delay between retries & # of retries
    radio.setRetries(15, 15);
    radio.setDataRate(RF24_250KBPS);
    radio.setChannel(120);

    // Open pipes to other nodes for communication
    radio.openWritingPipe(ac2_send);
    radio.openReadingPipe(1, ac2_receive);

    // Start listening
    radio.startListening();

    // Dump the configuration of the rf unit for debugging
    //radio.printDetails();

    pinMode(pin1, OUTPUT);
    digitalWrite(pin1, LOW);
    pinMode(pin2, OUTPUT);
    digitalWrite(pin2, LOW);
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

            // Put a zero at the end for easy printing
            receive_payload[payload_size] = 0;
            strcpy(send_payload, receive_payload);

            printf("Got payload. Size=%i, Value=%s\n", payload_size, receive_payload);

            if (receive_payload[1] == 'S' && strcmp(getDeviceID(receive_payload), DEVICE_ID) == 0)
            {
                switch (receive_payload[0])
                {
                    case 'S':
                        if (running)
                            send_payload[0] = 'O';
                        else
                            send_payload[0] = 'F';
                        break;

                    case 'O':
                        if (!running)
                            digitalWrite(pin1, HIGH);
                            digitalWrite(pin2, HIGH);

                        running = true;
                        send_payload[0] = 'O';
                        break;

                    case 'F':
                        if (running)
                            digitalWrite(pin1, LOW);
                            digitalWrite(pin2, LOW);

                        running = false;
                        send_payload[0] = 'F';
                        break;
                }

                send_payload[1] = 'R';

                // First, stop listening so we can talk
                radio.stopListening();

                // Send the final one back.
                radio.write(send_payload, payload_size);
                printf("Sent responses. Value=%s\n", send_payload);

                // Now, resume listening so we catch the next packets.
                radio.startListening();
            }
        }
    }
}
