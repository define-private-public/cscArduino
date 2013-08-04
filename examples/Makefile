# Makefile for CSC Arduino examples

# Compiler stuff
C = gcc
CFLAGS = -W

CXX = g++
CXXFLAGS = -W


# For CSC Arduino
CODE_FOLDER = ../code/
CSC_H = $(CODE_FOLDER)csc_arduino.h
CSC_C = $(CODE_FOLDER)csc_arduino.c
CSC_FILES = $(CSC_H) $(CSC_C)
SDL_LIBS = -lSDL
GL_LIBS = -lGL -lglut
GTKMM = `pkg-config gtkmm-3.0 --cflags --libs`


all: random_bytes pong rgb_led_driver mpu_data mpu_opengl_cube


random_bytes: $(CSC_FILES) random_bytes.c
	$(C) $(CFLAGS) -o random_bytes random_bytes.c $(CSC_C) -I$(CODE_FOLDER)


pong: $(CSC_FILES) pong.c
	$(C) $(CFLAGS) -o pong pong.c $(CSC_C) $(SDL_LIBS) -I$(CODE_FOLDER)


rgb_led_driver: $(CSC_FILES) rgb_led_driver.cpp
	$(CXX) $(CXXFLAGS) -o rgb_led_driver rgb_led_driver.cpp $(CSC_C) -I$(CODE_FOLDER) $(GTKMM)


mpu_data: $(CSC_FILES) mpu_data.c
	$(C) $(CFLAGS) -o mpu_data mpu_data.c $(CSC_C) -I$(CODE_FOLDER)


mpu_opengl_cube: $(CSC_FILES) mpu_opengl_cube.cpp $(GL_LIBS)
	$(CXX) $(CXXFLAGS) -o mpu_opengl_cube mpu_opengl_cube.cpp $(CSC_C) -I$(CODE_FOLDER) $(GL_LIBS)


clean:
	rm -f *.o


realclean: clean
	rm -f random_bytes pong rgb_led_driver mpu_data mpu_opengl_cube
