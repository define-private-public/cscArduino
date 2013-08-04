// File:         mpu_data.c 
// Author:       Benjamin  Summerton  <define-private-public@github.com>
// Description:  Retrives the raw data from an MPU 6050 chip.

#include "csc_arduino.h"
#include <signal.h>

/*== MPU 6050 related code ==*/

// Definitions
#define IS_ALIVE 0x1	// Unused (maybe for future purposes)
#define REQ_ALL 0x2		// Make a request for all of the MPU data

// For the MPU 6050 
typedef struct mpu_6050_t {
	char error;
	short x_accel;
	short y_accel;
	short z_accel;
	short x_gyro;
	short y_gyro;
	short z_gyro;
	short temp;
} mpu_6050;


// Convert the buffer data to a structure
// Return value is the error value from the buffer
// Normally I would use memcpy(), but there are alignment issues
int bufferToStruct(mpu_6050 *mpu, unsigned char *buffer) {
	// Safety checks
	if (mpu == NULL)
		return -1;
	else if (buffer == NULL)
		return -2;
	
	// Set ALL the data
	mpu->error = buffer[0];
	mpu->x_accel = buffer[1];
	mpu->x_accel <<= 8;
	mpu->x_accel |= buffer[2];
	mpu->y_accel = buffer[3];
	mpu->y_accel <<= 8;
	mpu->y_accel |= buffer[4];
	mpu->z_accel = buffer[5];
	mpu->z_accel <<= 8;
	mpu->z_accel |= buffer[6];
	mpu->x_gyro = buffer[7];
	mpu->x_gyro <<= 8;
	mpu->x_gyro |= buffer[8];
	mpu->y_gyro = buffer[9];
	mpu->y_gyro <<= 8;
	mpu->y_gyro |= buffer[10];
	mpu->z_gyro = buffer[11];
	mpu->z_gyro <<= 8;
	mpu->z_gyro |= buffer[12];
	mpu->temp = buffer[13];
	mpu->temp <<= 8;
	mpu->temp |= buffer[14];

	return mpu->error;
}



// Program gloval variables
cscArduino *arduino = NULL;

// Incase someone decided to interupt the terminal
void int_signal(int sig) {
	printf("Interupted! (How Rude.)\n");
	cscFreeConnection(arduino);
	printf("\n");
	exit(sig);
}


// Main program routine
int main(int argc, char *argv[]) {
	// Program variables
	mpu_6050 mpu;
	int i, k;
	unsigned char buffer[15];
	
	// connect the signal for an interrupt
	if (signal(SIGINT, int_signal) == SIG_ERR)
		exit(EXIT_FAILURE);

	// Wee need at least three command line arguments
	if (argc < 3) {
		printf("Program Usage:\n");
		printf("\t$ %s <port> <baudrate>\n\n", argv[0]);
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


	// Do a maximum of 1,000 readings
	i = 0;
	while (i < 1000) {
		// Get the number of available bytes
		k = cscAvailable(arduino);
		if (k == -1)
			perror("Couldn't see how many bytes are available.\n");
		else if (k == 15) {
			// Grab the bytes and put them into the structure
			cscRecv(arduino, buffer, 15);
			bufferToStruct(&mpu, buffer);

			if (mpu.error != 0)
				printf("DATA ERROR\n\n\n\n\n\n");
			else {
				// Success!  (print out raw data)
				printf("a_x=%i\n", mpu.x_accel);
				printf("a_y=%i\n", mpu.y_accel);
				printf("a_z=%i\n", mpu.z_accel);
				printf("g_x=%i\n", mpu.x_gyro);
				printf("g_y=%i\n", mpu.y_gyro);
				printf("g_z=%i\n", mpu.z_gyro);
			}
			
			i += 1;
		} else {
			// Else, nothing?  Send a request for a reading
			buffer[0] = REQ_ALL;
			cscSend(arduino, buffer, 1);
		}

		usleep(1000);	// Take a nap
	}

	// Exit the program, and cleanup
	cscFreeConnection(arduino);
	printf("\n");
	return 0;
}


