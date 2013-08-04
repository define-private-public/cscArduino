Introduction
------------

How you use the library is defined by you.  You can have it setup a connection,
then poll/send data in a loop, or have it do that I/O periodically as needed
(i.e. with signals).  For example, a basic structure may look like this:

    Setup connection to arudino
    ...

    Start connection w/ arduino
    ...
    Enter man program loop:
        ...
        Check for input
        If there is input:
            Grab X amount of bytes
            ...
        ...
    ...
    Close connection with Arduino
    ...
    Exit program

What you do doesn't have to look like this, but it might.


### Suppoerted Langauges
 * C (see `./c.md`)
 * C++ (see `./cpp.md`)


### Supported Operating Systems

Generally an POSIX compliant OS should work, but there may be some portability
issues.  This library was written on `Ubuntu Linux 13.04`, but has been tested
on `OS X 10.8`.

Windows is not supported as of right now, but there are plans to do so in the
future.


### Principles For Writing Good Serial Programs

 1. The first thing that I should mention is that it takes the Arduino's Serial
    communications is that it takes about 2 seconds to be able to properly
    communicate with the device.  This may seem a bit odd, but if you run some
	of the examples, you might see some input from the microcontroller, it will
	pause for a few seconds, then data transfer will resume.

 2. Make sure that nothing is being transfered when the application ends.
 	So say you have a program that properly transfers data, but the second time
	that you try to run it, it is not able to establish a communication or the
	data that you get is werid.  This usually happens because data might still
	be transfering over the line even after an application has ended.  I usually
	operate on a model where the C/C++ program will send a byte to request a
	certain packet of data, then the Arduino will responsed with the relevant
	readings; I consider that to be one "transaction."

 3. Try to use buffered I/O whenever possible.  It makes sending data much more
	efficient and faster.

 4. The Arduino has a 64 byte input buffer.  You can change this via some
	firware editing, but that is beyond the scope of this project.

 5. Your OS also has serial I/O buffers.  The size may vary, but the common
	number that I see is 4096 bytes.  This may be editable though on some
	systems, but you cannot change it via the library as of now.

 6. The library was built around using Non-Canonical communications.  What this
    means is that the underlying operating system will not touch any of the
	incoming our outgoing data.  In Canonical mode, certain bytes are given
	special meanings (such as the newline character "\n"), and then are
	processed by the OS.  If you know what you are doing, you can edit the
	library's code to use Canonical I/O.

