// File:         csc_arduino.h
// Author:       Benjamin Summerton  <define-private-public@github.com>
// Description:  Function declarations for CSC Arduino wrapper library.
// Version:      v1.0

#ifndef CSC_ARDUINO_H
#define CSC_ARDUINO_H 1

#define CSC_ARDUINO_VERSION "v1.0"

#define CSC_DEFAULT_CLOSE_SLEEP_DUR 1000000

// C Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// POSIX includes
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

// Definitions the library uses

// Basic datastruture
typedef struct cscArduino_t {
	int fd;
	struct termios old_tio;
	struct termios tio;
	char *port;
	int baudrate;
	useconds_t close_sleep_dur;
} cscArduino;


// function prototypes
cscArduino *cscSetupConnection(const char *port, int baudrate);
void cscFreeConnection(cscArduino *tty);
int cscStartConnection(cscArduino *tty);
// TODO: Stop connection? (instead of free)  (or at least pause?)
int cscRecv(cscArduino *tty, unsigned char *bytes, int num);
int cscSend(cscArduino *tty, unsigned char *bytes, int num);
int cscFlushInput(cscArduino *tty);
int cscFlushOutput(cscArduino *tty);
int cscFlush(cscArduino *tty);
int cscAvailable(cscArduino *tty);
int cscPending(cscArduino *tty);
int cscWaitForSend(cscArduino *tty);


// If we are using C++, add in the OO Wrapper
#ifdef __cplusplus

namespace csc {
	class Arduino {
		public:
			// Constructors
			// They replace `cscSetupConnection()` and 'cscFreeConnection()`
			Arduino(const char *port, int baudrate);
			~Arduino();

			// Wrapper methods
			int startConnection();
			int recv(unsigned char *bytes, int num);
			int send(unsigned char *bytes, int num);
			int flushInput();
			int flushOutput();
			int flush();
			int available();
			int pending();
			int waitForSend();

			// Extra methods
			bool isConnectionSetup();
			int fd();
			char *port();
			int baudrate();
			useconds_t getSleepDur();
			void setSleepDur(useconds_t new_dur);
			
		private:
			cscArduino *_tty;
	};
}

#endif // __cplusplus


#endif // CSC_ARDUINO_H

