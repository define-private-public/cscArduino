// File:         csc_arduino.c 
// Author:       Benjamin Summerton  <define-private-public@github.com>
// Description:  Function defnitions for CSC Arduino wrapper library.
// Version:      v1.0

#include "csc_arduino.h"

// Sees if we can connect to an arduino
cscArduino *cscSetupConnection(const char *port, int baudrate) {
	// Variables
	cscArduino *tty = NULL;

	// TODO: code to check if something is a terminal or not

	// First get some memory allocated
	tty = (cscArduino *)malloc(sizeof(cscArduino));
	if (tty == NULL) {
		// Print an error message and return NULL
		fprintf(stderr, "Could not allocate memory for arduino structore, w/ port: %s\n", port);
		return NULL;
	}

	// Then try to connect to the device
	tty->fd = open(port, O_RDWR | O_NOCTTY);	
	if (tty->fd == -1) {
		// Print error
		fprintf(stderr, "Could not connect to port: %s\n", port);

		// Free the structure and return NULL
		cscFreeConnection(tty);
		return NULL;
	}
	tty->port = ttyname(tty->fd);

	// Get the current termios structure and store it
	tcgetattr(tty->fd, &tty->old_tio);

	// Set the termios (to a raw mode)
	// TODO: Add more options for terminal control
	memset(&tty->tio, 0, sizeof(tty->tio));
	tty->tio.c_iflag = 0;
	tty->tio.c_oflag = 0;
	tty->tio.c_cflag = CS8 | CREAD | CLOCAL; // 8n1
	tty->tio.c_lflag = 0;
	tty->tio.c_cc[VMIN] = 0;
	tty->tio.c_cc[VTIME] = 0;
	
	// Set the baud rate
	tty->baudrate = baudrate;	// Store it in the arduino structure
	switch (baudrate) {
		// NOTE: B0 (or '0') is not specified here because it would caus a hangup
		case 50:
			baudrate = B50;
			break;
		case 75:
			baudrate = B75;
			break;
		case 110:
			baudrate = B110;
			break;
		case 134:
			baudrate = B134;
			break;
		case 150:
			baudrate = B150;
			break;
		case 200:
			baudrate = B200;
			break;
		case 300:
			baudrate = B300;
			break;
		case 600:
			baudrate = B600;
			break;
		case 1200:
			baudrate = B1200;
			break;
		case 1800:
			baudrate = B1800;
			break;
		case 2400:
			baudrate = B2400;
			break;
		case 4800:
			baudrate = B4800;
			break;
		case 9600:
			baudrate = B9600;
			break;
		case 19200:
			baudrate = B19200;
			break;
		case 38400:
			baudrate = B38400;
			break;
		case 57600:
			baudrate = B57600;
			break;
		case 115200:
			baudrate = B115200;
			break;
		case 230400:
			baudrate = B230400;
			break;
		defualt:
			// TODO: Change this for some of the other baudrates like `14400`
			fprintf(stderr, "Error: Baudrate `%i` is not valid.\n", baudrate);
			cscFreeConnection(tty);
			return NULL;
	}

	// Set the output speed
	if (cfsetospeed(&tty->tio, baudrate) == -1) {
		// Print an error message
		fprintf(stderr, "Error, could not set output baudrate for device on port: %s.\n", port);
		cscFreeConnection(tty);
		return NULL;
	}

	// Set the input speed
	if (cfsetispeed(&tty->tio, baudrate) == -1) {
		// print an error message
		fprintf(stderr, "Error, could not set input baudrate for device on port: %s\n", port);
		cscFreeConnection(tty);
		return NULL;
	}

	// And set the close sleep duration
	tty->close_sleep_dur = CSC_DEFAULT_CLOSE_SLEEP_DUR;

	// All else should have gone fine, return the structure
	return tty;
}


// cleans up the structure and flushes stuff for us
void cscFreeConnection(cscArduino *tty) {
	if (tty == NULL)
		return;

	// Flush, reset termios, and close the fd
	cscFlush(tty);
	tcsetattr(tty->fd, TCSANOW, &tty->old_tio);
	usleep(tty->close_sleep_dur);
	close(tty->fd);

	/* The `usleep()` call is up there because sometimes the arduino needs to
	   stop  sending data over the line when we are closing the conneciton.
	   So far, this seems to be what works, but I am investigating a better
	   way of clearing out the line.*/

	// then the rest of the structure
	free(tty);
}


// finally connect to the thing
int cscStartConnection(cscArduino *tty) {
	if (tty == NULL)
		return -1;
	
	// TODO: place lock on file before `startconnection` is called
	return tcsetattr(tty->fd, TCSANOW, &tty->tio);
}


// Gets a vew bytes
int cscRecv(cscArduino *tty, unsigned char *bytes, int num) {
	if (tty == NULL)
		return -1;
	
	return read(tty->fd, bytes, num);
}


// Sends a few bytes to the arduino
int cscSend(cscArduino *tty, unsigned char *bytes, int num) {
	int n;
	if (tty == NULL)
		return -1;
	
	n = write(tty->fd, bytes, num); // send em

	// did we write 0 or more bytes?
	if (n >= 0)
		return n;
	
	// else, there was an error
	return -1;
}


// Flushes the input buffer
int cscFlushInput(cscArduino *tty) {
	if (tty == NULL)
		return -1;
	
	// tcflush() here would return an error if the thing isn't TTY
	return tcflush(tty->fd, TCIFLUSH);
}


// Flushes the output buffer
int cscFlushOutput(cscArduino *tty) {
	if (tty == NULL)
		return -1;

	return tcflush(tty->fd, TCOFLUSH);
}


// Flushes both the input and output buffers
int cscFlush(cscArduino *tty) {
	if (tty == NULL)
		return -1;
	
	return tcflush(tty->fd, TCIOFLUSH);
}


// Gets the number of available bytes
int cscAvailable(cscArduino *tty) {
	int i = 0;

	if (tty == NULL)
		return -1;

	// See how many bytes are waiting in the input buffer
	if (ioctl(tty->fd, FIONREAD, &i) == -1)
		return -1;
	else
		return i;
}


// Gets the number of bytes that haven't been send to the Arduino yet
int cscPending(cscArduino *tty) {
	int i = 0;

	if (tty == NULL)
		return -1;

	// See how many bytes are waiting in the input buffer
	if (ioctl(tty->fd, TIOCOUTQ, &i) == -1)
		return -1;
	else
		return i;
}


// Holds the process until all the bytes in the output buffer have been sent
int cscWaitForSend(cscArduino *tty) {
	if (tty == NULL)
		return -1;

	return tcdrain(tty->fd);
}


/* You should never to this in a *.c file, but I put this here so a user
   doesn't have to inculde a .cpp file for the library, this could change
   though in the future if I discover some error. */
#ifdef __cplusplus

namespace csc {
	// Class constructor (a.k.a cscSetupConnection())
	Arduino::Arduino(const char *port, int baudrate) :
		_tty(NULL)
	{
		// Setup the connection
		_tty = cscSetupConnection(port, baudrate);
	}


	// class deconstructor (a.k.a cscFreeConnection)
	Arduino::~Arduino() {
		cscFreeConnection(_tty);
	}
	

	// Initiate the conneciton
	int Arduino::startConnection() {
		return cscStartConnection(_tty);
	}


	// Receive Bytes
	int Arduino::recv(unsigned char *bytes, int num) {
		return cscRecv(_tty, bytes, num);
	}


	// Send bytes to the device
	int Arduino::send(unsigned char *bytes, int num) {
		return cscSend(_tty, bytes, num);
	}


	// Flush the input buffer
	int Arduino::flushInput() {
		return cscFlushInput(_tty);
	}


	// Flush the output buffer
	int Arduino::flushOutput() {
		return cscFlushOutput(_tty);
	}


	// Flush both the input and output buffers
	int Arduino::flush() {
		return cscFlush(_tty);
	}


	// Check how  many bytes are availble in the input buffer
	int Arduino::available() {
		return cscAvailable(_tty);
	}


	// Get a count of the bytes in the output buffer
	int Arduino::pending() {
		return cscPending(_tty);
	}

	
	// Block the process until the bytes we wrote with `recv()` have been sent
	int Arduino::waitForSend() {
		return cscWaitForSend(_tty);
	}


	/*== Extra Methods ==*/

	// See if the TTY channel is open or not.
	bool Arduino::isConnectionSetup() {
		return (_tty != NULL);
	}
	
	// Returns the file descriptor for the Arduino device
	int Arduino::fd() {
		if (_tty == NULL)
			return -1;

		return _tty->fd;
	}

	// Retruns a pointer to the filename of the arduino.  You do not need to
	// free this from memory at all
	char *Arduino::port() {
		if (_tty == NULL)
			return NULL;

		return _tty->port;
	}

	// Retruns the buadrate that is currently being used.
	int Arduino::baudrate() {
		if (_tty == NULL)
			return -1;

		return _tty->baudrate;
	}
	
	// Get the close sleep duration 
	useconds_t Arduino::getSleepDur() {
		if (_tty == NULL)
			return 0;

		return _tty->close_sleep_dur;
	}

	// Change the close sleep duration
	void Arduino::setSleepDur(useconds_t new_dur) {
		if (_tty == NULL)
			return;

		_tty->close_sleep_dur = new_dur;
	}
}

#endif // __cplusplus

