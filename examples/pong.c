// File:         pong.c
// Author:       Benjamin  Summerton 
// Description:  The classic game of Pong, as controlled by potentiometers on
//               an Arduino.  Credit to Atari for making the game.
//
//               Please note that you will need SDL to compile this program.

// Includes
#include "csc_arduino.h"
#include <signal.h>
#include <SDL/SDL.h>

#define SCREEN_W 800
#define SCREEN_H 600
#define PADDLE_W 16
#define PADDLE_H 96
#define GOAL_W 32
#define BALL_S 16
#define HITS_TILL_INC 10
#define VEL_START 2
#define VEL_INC 1
#define VEL_PADDLE 4

#define POT_MAX 1023
#define REQ 2


// Needs to be in the global space	
cscArduino *arduino = NULL;


// Prints a SDL_Rectangle values;  (was used for debugging)
inline void printRect(SDL_Rect *r) {
	printf("x:%i, y:%i, w:%i, h:%i\n", r->x, r->y, r->w, r->h);
}


// Sees if two SDL_Rect's are colliding
Uint8 collide(SDL_Rect a, SDL_Rect b) {
	// Check bottoms and tops
	if ((a.y + a.h) < b.y)
		return 0;
	if (a.y > (b.y + b.h))
		return 0;
	
	// Check right and lefts
	if ((a.x + a.w) < b.x)
		return 0;
	if (a.x > (b.x + b.w))
		return 0;
	
	return 1;
}


// Incase someone decided to interupt the terminal
void int_signal(int sig) {
	printf("Interupted! (How Rude.)\n");
	SDL_Quit();
	cscFreeConnection(arduino);
	exit(sig);
}



// Main function
int main(int argc, char *argv[]) {
	// Program variables
	unsigned char buffer[4];	// Used for data transimssions
	Uint16 leftPot, rightPot;	// Left & Right potentiometers
	int i;

	Uint8 quit = 0;
	SDL_Surface *screen = NULL;
	SDL_Event event;
	Uint32 fg, bg;		// Colors

	SDL_Rect left, right, ball;
	Sint8 xVel, yVel;	// For the ball
	Uint8 counter;
	Uint8 scoreLeft, scoreRight;

	// We need three arguments
	// <programName> <port> <baudrate>
	if (argc < 3) {
		printf("Need at least two arguments:\n");
		printf("Usage: %s <port> <baudrate>\n", argv[0]);
		exit(1);
	}
	
	// connect the signal for an interrupt
	if (signal(SIGINT, int_signal) == SIG_ERR)
		exit(1);

	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		exit(1);
	
	// Setup the conneciton
	arduino = cscSetupConnection(argv[1], atoi(argv[2]));
	if (arduino == NULL)
		exit(1);

	// Set the caption
	SDL_WM_SetCaption("pong", NULL);

	// Set the Screen
	screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_SWSURFACE);
	if (screen == NULL)
		exit(1);
	
	// Fill up some data
	fg = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	bg = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	ball.x = (SCREEN_W / 2) - (BALL_S / 2);
	ball.y = (SCREEN_H / 2) - (BALL_S / 2);
	ball.w = ball.h = BALL_S;
	xVel = yVel = VEL_START;
	counter = 0;

	leftPot = rightPot = 400;	// Start in middle-ish
	left.w = right.w = PADDLE_W;
	left.h = right.h = PADDLE_H;
	left.x = GOAL_W;
	right.x = SCREEN_W - PADDLE_W - GOAL_W;
	left.y = right.y = ((float)leftPot / (float)POT_MAX) * SCREEN_H;
	scoreLeft = scoreRight = 0;
	
	// Connect to the arduino
	if (cscStartConnection(arduino) == -1)
		exit(1);
	
	// Hold the image there
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			// Check for a quit
			if (event.type == SDL_QUIT)
				quit = 1;
			if (event.type == SDL_KEYUP)
				if (event.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;
		}
		
		/*== Update logic ==*/
		// Get data from arduino and use it to update the paddles
		i = cscAvailable(arduino);
		if (i >= 4) {
			// Get the bytes and put them into their correc potentiometers
			cscRecv(arduino, buffer, 4);
			leftPot = buffer[0];
			leftPot <<= 8;
			leftPot |= buffer[1];
			rightPot = buffer[2];
			rightPot <<= 8;
			rightPot |= buffer[3];

			left.y = ((float)leftPot / (float)POT_MAX)
						* (SCREEN_H - left.h);
			right.y = ((float)rightPot / (float)POT_MAX)
						* (SCREEN_H - right.h);
		} else {
			buffer[0] = REQ;
			cscSend(arduino, buffer, 1);
		}

		// See if we should increase the speed
		if (counter >= HITS_TILL_INC) {
			counter = 0;	// reset
			
			// Increse depending upon the direction
			if (xVel > 0)
				xVel += VEL_INC;
			else
				xVel -= VEL_INC;

			if (yVel > 0)
				yVel += VEL_INC;
			else
				yVel -= VEL_INC;;
		}

		// Change the position
		ball.x += xVel;
		ball.y += yVel;

		// Check bounce w/ top walls
		if (ball.y <= 0) {
			yVel *= -1;
			ball.y += yVel;
			counter++;
		} else if ((ball.y + ball.h) >= SCREEN_H) {
			yVel *= -1;
			ball.y += yVel;
			counter++;
		}


		// check for score & reset the ball
		if (ball.x <= 0) {
			scoreRight++;
			ball.x = (SCREEN_W / 2) - (BALL_S / 2);
			ball.y = (SCREEN_H / 2) - (BALL_S / 2);
			xVel = yVel = VEL_START;
			counter = 0;

			printf("Left:%i\t\tRight:%i\n", scoreLeft, scoreRight);
		} else if ((ball.x + ball.w) >= SCREEN_W) {
			scoreLeft++;
			ball.x = (SCREEN_W / 2) - (BALL_S / 2);
			ball.y = (SCREEN_H / 2) - (BALL_S / 2);
			xVel = yVel = -VEL_START;
			counter = 0;

			printf("Left:%i\t\tRight:%i\n", scoreLeft, scoreRight);
		}

		// Check for a paddle collison
		if (collide(ball, left)) {
			xVel *= -1;
			ball.x += xVel;
			counter++;
		} else if (collide(ball, right)) {
			xVel *= -1;
			ball.x += xVel;
			counter++;
		}


		/*== Drawing logic ==*/
		SDL_FillRect(screen, &screen->clip_rect, bg);
		SDL_FillRect(screen, &ball, fg);
		SDL_FillRect(screen, &left, fg);
		SDL_FillRect(screen, &right, fg);
		
		// Flip the screen
		if (SDL_Flip(screen) == -1)
			exit(1);
		
		SDL_Delay(10);	// Use less CPU power
	}

	SDL_Quit();
	cscFreeConnection(arduino);
	return 0;
}

