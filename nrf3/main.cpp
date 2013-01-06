/* 
 * File:   main.cpp
 * Author: purinda
 * 
 * This file has to be compiled and uploaded to the Beaglebone
 * the role of the application is to ping the arduino module
 * file the arduino sketch in 
 *
 * Created on 24 June 2012, 10:54 AM
 */

#include <cstdlib>
#include <iostream>
#include "lib/RF24/RF24.h"
#include "lib/RF24/compatibility.h"

using namespace std;


//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xACACACACD2LL, 0xACACACACE1LL };

//
// Payload
//
const int payload_size = 4;
char receive_payload[payload_size+1]; // +1 to allow room for a terminating NULL char

// CE and CSN pins On header P9 (P3.21 and P3.19)
RF24 radio(8, 25);  //only CSN is NEEDED in RPI

void setup(void)
{
    radio.begin();
    radio.setRetries(15,15);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setChannel(120);
    radio.setCRCLength(RF24_CRC_16);

    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);

    //
    // Start listening
    //
    //radio.startListening();

    //
    // Dump the configuration of the rf unit for debugging
    //
    //radio.printDetails();
}

bool loop(char* send_payload)
{
    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    int result;
    printf("Now sending length %i... ",payload_size);
    result = radio.write( send_payload, payload_size );
    printf("write = %d, ",result);

    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout
    long started_waiting_at = __millis();

    bool timeout = false;
    while ( ! radio.available() && ! timeout )
        if (__millis() - started_waiting_at > 500 )
            timeout = true;

    // Describe the results
    if ( timeout )
    {
        printf("Failed, response timed out.\n\r");
        return false;
    }
    else
    {
        // Grab the response, compare, and send to debugging spew
        radio.read( receive_payload, payload_size );

        // Put a zero at the end for easy printing
        receive_payload[payload_size] = 0;

        // Spew it
        printf("Got response size=%i value=%s\n\r",payload_size,receive_payload);
        char test_payload[payload_size +1];
        strcpy(test_payload, send_payload);
        test_payload[0] = 'R';

        if (strcmp(test_payload, receive_payload) != 0)
        {
            printf("Response Error\n");
            return false;
        }
        else
            return true;
    }
}


int main(int argc, char** argv) 
{
    /*
     * 100: ping test
     * 101: turn on lights
     * 102: turn off lights
     * 103: turn on fan
     * 104: turn off fan
     */
    if (argc != 2)
    {
        printf("Error Input\n");
        return 1;
    }

    setup();

    char send_payload[payload_size+1];
    int command;
    command = (int)atoi(argv[1]);

    switch (command)
    {
        case 100:
            strcpy(send_payload, "C100");
            break;
        case 101:
            strcpy(send_payload, "C101");
            break;
        case 102:
            strcpy(send_payload, "C102");
            break;
        case 103:
            strcpy(send_payload, "C103");
            break;
        case 104:
            strcpy(send_payload, "C104");
            break;
        default:
            printf("Error Input\n");
            return 0;
    }

    if (command == 100)
    {
        while(1)
        {
            loop(send_payload);
            sleep(1);
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            if (loop(send_payload))
                break;

            //usleep(500000);
        }
    }

    return 0;
}

