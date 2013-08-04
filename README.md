CSC Arduino (A POSIX Serial Wrapper Library)
============================================


License:
--------
Everything in this project adhears to a BSD 3-Clause license (unless noted 
elsewhere).  Please check the file `./LICENSE` for the full text of the license.


Author:
-------
The main author of this project is Ben Summerton (a.k.a `define-private-public`
on Github.com.  He is a student at RIT studying Computer Science.


Credits:
--------
I would like to give thanks to Tod (of [http://todbot.com/](Todbot.com)) and his
[2006 blog post about Arduino serial communications in
C](http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino/).
It was the starting point for this project.

Ciruit schematics were made with [Fritzing](http://fritzing.org/).


About:
------
CSC Arduino is a small set of functions that wraps over Kernel system calls on
POSIX compliant systems to connect to serial devices.  In layman's terms, it
makes it much easier to write C/C++ programs that can talk to an Arduino.

### Why?:
As much as I think that microcontrollers are pretty cool devices, I don't have
that much of an interest in them, other than gathering data.  I also love
writing C/C++ programs more than fiddling around with electronics.  I had a
problem though; I couldn't easily send data from an Arduino to one of my C/C++
applications.  I eventually did find a tutorial online explaining how to setup
serial communications (on a POSIX system) in C, yet I didn't think it was as
simple as it could be.

In January


How-To:
--------
The "library," is found in the `./code/` directory.  Simply copy all of the files into whatever project you are working on, `#include` the header file into your
source code and add in the C source file into your compilation arguements.


Documentation:
--------------
All documentation for this project should be in the `./docs/` folder.  For
explanations on the example programs, check `./examples/README.md`.


Example Code:
-------------
All of the example code is in the folder `./examples/`.  This includes C/C++
source code of examples, Arduino sketches for examples, as well as the circuit
schematics (found in `./examples/circuit_schematics/`).

Please read the `./examples/README.md` file for more information about compiling
and running the applications.


