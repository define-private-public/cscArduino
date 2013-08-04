How to compile & run the example applications.
----------------------------------------------


### Quick How-To

#### Compiling

To compile all of the example programs, simply run the `Makefile` by typing:

    $ make

You can also make specific examples by specifying the program name as the
target.  E.g.:

    $ make pong

Some of the demos may require the following libraries:
 * [libSDL](http://libsdl.org)
 * [freeglut](http://freeglut.sourceforge.net) (or any other GLUT interface).
 * [gtkmm](http://gtkmm.org/)

- - -

#### Hardware

This is a list of some of the hardware you will need for the examples:

 * An Arduino (I used an Uno R3) and USB serial cable
 * 8 Red LEDS
 * 8 100 Ohm resistors
 * 2 10K Ohm potentiometers
 * A Tri-Color RGB LED (or three separate RGB leds could do)
 * An `MPU 6050` chip (I used one with a `GY-521`)

- - -
 
#### Circuit Schematics

You can find all of the circuit design schematics in the `./circuit_schematics/`
folder.  There are image files (int PNG format,) of traditional circuit design
and "Bread Board," view.  Also included are the `*.fzz` design files, so you can
edit or look at them as you need (requires a separate application
[Fritzing](http://fritzing.org/)).

- - -

- - -


### List of Sample Applications

On of the left side of each arrow is a desktop application and everything to the
right of it is its corresponding Arduino sketch.  (`.ino` files are found in the
sub directories of the same names).

 * `random_bytes` -> `binary_leds`

   Sends a user defined count (with a user defined delay) of random bytes to the
   Arduino.  The microcontroller then turns on/off a set of 8 LEDS of the byte's
   binary representation.

   Usage:
       
        $ ./random_bytes <port> <baudrate> <num_transmissions> <u_delay>

    * `port` must be the locaitonof your Arduino on the filesystem.  E.g.
      `/dev/ttyACM0`.

    * `baudrate` needs to be set to `9600`, though you can change it by editing
      the Arduino sketch `binary_leds`.

    * `num_transmissions` is the number of random bytes you wish to send.  I
      would suggest something like `100` to start off.

    * `u_delay` is the desired microsecond delay between transfers.  Try
      `100000` first.

   Example usage:
   
        $ ./random_bytes /dev/ttyACM0 9600 100 100000

   - - -

 * `pong` -> `pong_controller`

   The classic game of Pong relived via Arduino and libSDL (which is something
   you will need installed for the demo application).

   Usage:

        $ ./pong <port> <baudrate>

    * `port` is the device file for your Arduino.  E.g. `/dev/ttyACM0`

    * `buadrate` must be `9600`, tough you can change this in the
      `pong_controller` sketch.

   Example usage:
   
        $ ./pong /dev/ttyACM0 9600

   - - -

 * `rgb_led_driver` -> `rgb_led`

   Control a Tri-Color LED via some sliders.  Requires gtkmm (the GTK+ C++ API)
   to run and compile.
   
   Unlike the other applications where you can provide the Arduino device file
   as a command line argument, this one the location is hard coded into the
   source file.  You will need to edit `rgb_led_driver.cpp` manually and find
   the value `/dev/ttyACM0` and change it.  I am sorry and appologize for this,
   but I do not (yet) know how to write proper GTK+ applications.

   Usage:

        $ ./rgb_led_drvier

   - - -

 * `mpu_data` -> `mpu_6050`

   Polls the Arduino for data from an Accelerometer chip (an `MPU 6050`).  Thie
   example requires that you have breakout board like a `GY-521`.

   I would also suggest setting your terminal's height to 7 lines.

   Usage:

        $ ./mpu_data <port> <baudrate>

    * `port` is the device file for your Arduino. E.g. `/dev/ttyACM0`.

    * `baudreate` must be `115200`, though you can change this in the `mpu_6050`
      sketch.

   Example Usage:

        $ ./mpu_data /dev/ttyACM0 115200

   - - -

 * `mpu_opengl_cube` -> `mpu_6050`

   Similar in functionality to the `mpu_data` example, except that it tries to
   make sense of the data and uses the gyroscope to rotate an 3D cube.  Requires
   both `OpenGL` and `glut` to compile and run.
   [freeglut](http://freeglut.sourceforge.net/) was used for development.

   I don't know exactly how to properly interpret accelerometer and gyroscope
   data yet, so there is some drift.

   When the program is running, make sure not to hold the Arduino and hold the
   `c` key for a second or two and that should make a calibration reading.

   If you ever want to reset the cube to its original position, press `r`

   Usage:

        $ ./mpu_opengl_cube <port> <baudrate>

    * `port` is the device file for your Arduino. E.g. `/dev/ttyACM0`.

    * `baudreate` must be `115200`, though you can change this in the `mpu_6050`
      sketch.

   Example Usage:

        $ ./mpu_opengl_cube /dev/ttyACM0 115200

   - - -


