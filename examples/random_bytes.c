// File:         random_bytes.c
// Author:       Benjamin  Summerton  <define-private-public@github.com>
// Description:  This is to be used with the "binary_leds," Arduino sketch.
//               What it will do is take in a 4th command line argument being
//               how many times we should generte a random number and send it
//               to the Arduino.  It will then send the byte back to us.


#include "csc_arduino.h"
#include <time.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0

// Global variables
cscArduino *arduino = NULL;


// print out an 8-bit integer to stdout
void print_binary(unsigned char byte) {
	unsigned char i, j;

	for (i = 0, j = 128; i < 8; i++, j >>= 1)
		printf("%i", (byte & j) ?  1 : 0);
}


// Terminal interrupt
void int_signal(int sig) {
	printf("Interupted! (How Rude.)\n");
	cscFreeConnection(arduino);
	printf("\n");
	exit(sig);
}


// Main program routine
int main(int argc, char *argv[]) {
	int num_trans = 0, delay = 0;
	unsigned char buffer[1], last_byte = 0, send_ok = TRUE;
	int i, j;

	// Check for command line arguments
	if (argc < 5) {
		printf("Usage:\n");
		printf("%s <port> <baudrate> <num_transmissions> <u_delay>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));			// So we get random-ish numbers
	num_trans = atoi(argv[3]);
	delay = atoi(argv[4]);

	// Setup the arduino 
	arduino = cscSetupConnection(argv[1], atoi(argv[2]));
	if (arduino == NULL) {
		fprintf(stderr, "Could not setup connection to Arduino.\n");
		exit(EXIT_FAILURE);
	}

	// Setup the connection
	arduino = cscSetupConnection(argv[1], atoi(argv[2]));
	if (arduino == NULL) {
		perror("Could not setup conneciton.\n");
		exit(EXIT_FAILURE);
	}

	// Start the connection
	if (cscStartConnection(arduino) == -1) {
		perror("Could not start the connection.\n");
		cscFreeConnection(arduino);
		exit(EXIT_FAILURE);
	}

	// For some reason we have to wait a second or two for the connection
	// to be ready.  I am investigation this issue (may be solved w/ using
	// the poll and select APIs.  For now, it will just wait two  seconds.
	sleep(2);

	// Main loop
	i = 0;
	while (i < num_trans) {
		j = cscAvailable(arduino);	// Get the bytes in queue

		if (j == -1)
			fprintf(stderr, "Error reading bytes in input queue.\n");
		else if (j >= 1) {
			// Print out the byte we sent/received,
			cscRecv(arduino, buffer, 1);
			print_binary(buffer[0]);
			printf(" = %i", buffer[0]);

			// Just to make sure the data is the same.
			if (buffer[0] != last_byte)
				printf("  [BYTES WERE NOT THE SAME!]");

			printf("\n");
			i += 1;
			send_ok = TRUE;
		} else if (send_ok == TRUE){
			// Send it a random number
			buffer[0] = (unsigned char)(rand() % 256);
			cscSend(arduino, buffer, 1);
			last_byte = buffer[0];
			send_ok = FALSE;
		}

		usleep(delay);		// Take a nap
	}
	
	cscFreeConnection(arduino);
	return EXIT_SUCCESS;
}

