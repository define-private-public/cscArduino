Documentation for use with C++
------------------------------


### Classes

The C++ wrapper only contains one class, `Arduino` which is located in the `CSC`
namespace.  Most of its members are functions that just wrap over the C
functions, but there are a few extras.


### Location of Declarations and Defintions

The class declaration of `csc::Arduino` is inside of the `csc_arduino.h` and the
declaration (which is what you will include during compilation time) is located
in `csc_arduino.c`.

- - -


### Constructor

 * `Arduino::Arduino(const char *port, int baudrate)`

   This is sole constructor for the class.  It will setup the connection to the
   Arduino.  It is the equivalent of `cscSetupConnection()`.

    * `port`: Location of device file in filesystem.  E.g. `/dev/ttyACM0`.

    * `buadrate`: Buadrate at which you want to communicate at.  Most of the
       Arduino buadrates are suppored (sans a few), but check the function
       definition of `cscSetupConnection()` for a complete list.

   - - -


### Deconstructor

 * `Arduino::~Arduino()`

   This will deallocate any resources and free the connection to the Arduino.
   It is equivalent of `cscFreeConnection()`.

   - - -


### Wrapper Methods

All of the wrapper methods have a 1-to-1 correspondance with the C API, except
that you don't need to pass in a pointer to a `cscArduino` datatype/structure.

 * `int Arduino::startConnection()`

   Will start the connection with the device.  Will return `0` on sucess or `-1`
   on failure.

   - - -

 * `int Arduino::recv(unsigned char *bytes, int num)`
   
   Reads in bytes from the Arduino.  On sucess, will return number of bytes read
   in. On failure, will return `-1`.

    * `bytes`: Buffer of where to store the read in bytes.

    * `num`: Desired number of bytes to read in.

  - - -

 * `int Arduino::send(unsigned char *bytes, int num)`

   Send bytes to the Arduino.  On sucess, will return number of bytes sent. On
   failure, will return `-1`.

    * `bytes`: Buffer of bytes that you want to send.

    * `num`: Desired number of bytes to send.

   - - -

 * `int Arduino::flushInput()`

   Flushes the input bufer.  On success will return `0`.  On failture, `-1`.

   - - -

 * `int Arduino::flushOutput()`

   Flushes the output bufer.  On success will return `0`.  On failture, `-1`.

   - - -

 * `int Arduino::flush()`

   Flushes both the input and output buffers.  On success will return `0`.
   On failure, `-1`.

   - - -

 * `int Arduino::available()`

   On success, this will return the number of bytes that are inside the input
   buffer (i.e. "what you have not read in yet from `cscRecv()`).  On failure it
   will return `-1`.

   - - -

 * `int Arduino::pending()`

   On sucess, this will return the number of byres that are waiting to be send
   to the Arduino.  On failure, this will return `-1`.

   - - -

 * `int Arduino::waitForSend()`

   This function will hold/block the process from continuing until bytes that
   you have tried to send with `cscSend()` have been recieved.  It can be a very
   useful fuction, but improper use can stop program executing (and might have
   to be force killed).

   On success it will return `0`; on failure `-1`.

   - - -


### Extra Methods

The here are ones that are not found in the C API, but are necessary for proper
usage of the library.

 * `bool Arduino::isConnectionSetup()`

   Will return `true` if we have successfully connected to the Arduino, `false`
   otherwise.

   - - -
   
 * `int Arduino::fd()`

   Returns the UNIX file descriptor of the Arduino Device.  If the connection
   was not setup properly, it will return `-1`.

   - - -

 * `char *Arduino::port()`
   
   Returns a pointer to the filename of the Arduino (on the file system).  If
   the connection wasn't setup properly, it will return `NULL`.

   - - -

 * `int Arduino::baudrate()`

   Returns the buadrate that was provided during instantiation of the class.  If
   the connection wasn't setup properly, it will return `-1`.

   - - -

 * `useconds_t Arduino::getSleepDur()`

   Returns the number of microseconds that the process will sleep for when
   freeing the connection.  If the connection wasn't setup properly, it will
   return `0`.

   - - -

 * `void Arduino::setSleepDur(useconds_t new_dur)`

   Sets the duration for which the process will sleep when freeing the
   connection.

    * `new_dur`: how long to sleep for (in microseconds).  E.g. `1000000`

   - - -

