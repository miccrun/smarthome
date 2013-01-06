/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

const int light = 5;
const int fan = 7;

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);
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

void setup(void)
{
  // Print preamble
  //

  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/pingpair_dyn/\n\r");
  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(120);
  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  //radio.printDetails();
  
  pinMode(light, OUTPUT);     
  pinMode(fan, OUTPUT);  
  
  digitalWrite(light, HIGH);
  digitalWrite(fan, HIGH);  

}

void loop(void)
{

    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
	done = radio.read( receive_payload, payload_size );

	// Put a zero at the end for easy printing
	receive_payload[payload_size] = 0;

	// Spew it
	printf("Got payload size=%i value=%s\n\r",payload_size,receive_payload);

        if (receive_payload[3] == '1')
        {
          digitalWrite(light, LOW);
        }
        else if (receive_payload[3] == '2')
        {
          digitalWrite(light, HIGH);
        }
        else if (receive_payload[3] == '3')
        {
          digitalWrite(fan, LOW);
        }
        else if (receive_payload[3] == '4')
        {
          digitalWrite(fan, HIGH);
        }
      }

      // First, stop listening so we can talk
      radio.stopListening();
      
      receive_payload[0] = 'R';
      // Send the final one back.
      radio.write( receive_payload, payload_size );
      printf("Sent response %s.\n\r",receive_payload);

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }

}
// vim:cin:ai:sts=2 sw=2 ft=cpp
