Documentation for use with C
----------------------------


### Data structures

CSC Arduino uses ones main datatypes/strucutre.

    typedef struct cscArduino_t {
        int fd;
        struct termios old_tio;
        struct termios tio;
        char *port;
        int baudrate;
        useconds_t close_sleep_dur;
    } cscArduino;

You should never make your own instance of it.  Instead, use the factory method
`cscSetupConnection()`.

`fd`, `port`, and `baudrate`, are memebers that you should be able to access if
you need to, but *you should never change*.

`close_sleep_dur` describes how many microsconds the program will pause for when
the connection to the Arduino is closed.  By default, it is set to `1,000,000`
(which is 1 second).  For most cases, you will not need to edit it, but it is
here if you need to.

`old_tio` and `tio` are members that you should not touch at all.


### Functions

 * `cscArduino *cscSetupConnection(const char *port, int buadrate)`
    
   Initializes the connection to the Arduino.  On success it will return a
   pointer to a `cscArduino` structure.  On failure it will return `NULL`.

    * `port`: Location of device file in filesystem.  E.g. `/dev/ttyACM0`.

    * `buadrate`: Buadrate at which you want to communicate at.  Most of the
       Arduino buadrates are suppored (sans a few), but check the function
       definition for a complete list.

   - - -

 * `void cscFreeConnection(cscArduino *tty)`

   Ends a connection and frees the cscArduino struct from memory.

    * `tty`: Pointer to a `cscArduino` struct.

   - - -

 * `int cscStartConnection(cscArduino *tty)`

   Starts the connection with the device.  Returns `0` on success or `-1` on
   failure.

    * `tty`: Pointer to a `cscArduino` struct.

   - - -
 
 * `int cscRecv(cscArduino *tty, unsigned char *bytes, int num)`

   Reads in bytes from the Arduino.  On sucess, will return number of bytes read
   in. On failure, will return `-1`.

    * `tty`: Pointer to a `cscArduino` struct.

    * `bytes`: Buffer of where to store the read in bytes.

    * `num`: Desired number of bytes to read in.

   - - -
 
 * `int cscSend(cscArduino *tty, unsigned char *bytes, int num)`

   Send bytes to the Arduino.  On sucess, will return number of bytes sent. On
   failure, will return `-1`.

    * `tty`: Pointer to a `cscArduino` struct.

    * `bytes`: Buffer of bytes that you want to send.

    * `num`: Desired number of bytes to send.

   - - -

 * `int cscFlushInput(cscArduino *tty)`

   Flushes the input bufer.  On success will return `0`.  On failture, `-1`.

    * `tty`: Pointer to a `cscArduino` struct.

   - - -

 * `int cscFlushOutput(cscArduino *tty)`

   Flushes the output bufer.  On success will return `0`.  On failture, `-1`.

    * `tty`: Pointer to a `cscArduino` struct.

   - - -

 * `int cscFlush(cscArduino *tty)`
   
   Flushes both the input and output buffers.  On success will return `0`.
   On failure, `-1`.

    * `tty`: Pointer to a `cscArduino` struct.

   - - -

 * `int cscAvailable(cscArduino *tty)`

   On success, this will return the number of bytes that are inside the input
   buffer (i.e. "what you have not read in yet from `cscRecv()`).  On failure it
   will return `-1`.

    * `tty`: Pointer to a `cscArduino` struct.

   - - -

 * `int cscPending(cscArduino *tty)`

   On sucess, this will return the number of byres that are waiting to be send
   to the Arduino.  On failure, this will return `-1`.

    * `tty`: Pointer to a `cscArduino` struct.

   - - -

 * `int cscWaitForSend(cscArduino *tty)

   This function will hold/block the process from continuing until bytes that
   you have tried to send with `cscSend()` have been recieved.  It can be a very
   useful fuction, but improper use can stop program executing (and might have
   to be force killed).

   On success it will return `0`; on failure `-1`.

    * `tty`: Pointer to a `cscArduino` struct.

   - - -

