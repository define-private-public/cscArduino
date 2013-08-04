// File:         mpu_opengl_cube.cpp
// Author:       Benjamin  Summerton  <define-private-public@github.com>
// Description:  Rotate a 3D cube (rendered in OpenGL) with a MPU 6050
//
//               Will require the GLUT library (I recommend using freeGLUT).

#include "csc_arduino.h"
#include <signal.h>
#include <math.h>
#include <vector>
using std::vector;

// GLUT
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif


// Definitions
#define IS_ALIVE 0x1	// Unused (maybe for future purposes)
#define REQ_ALL 0x2		// Make a request for all of the MPU data


/*== MPU 6050 structs and functions ==*/
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


// Conver the buffer data to a structure
// Return value is the error value from the buffer
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


/** Data structures (for OpenGL cube mesh)**/
struct color3f {
	float r;
	float g;
	float b;
	
	color3f (float red, float green, float blue) {
		r = red;
		g = green;
		b = blue;
	}
};

struct vertex3f {
	float x;
	float y;
	float z;
	color3f *clr;
	
	vertex3f (float xPos, float yPos, float zPos, color3f *color) {
		x = xPos;
		y = yPos;
		z = zPos;
		clr = color;
	}
};

struct polygon3 {
	vertex3f *v1;
	vertex3f *v2;
	vertex3f *v3;
	
	polygon3 (vertex3f *vertex1, vertex3f *vertex2, vertex3f *vertex3) {
		v1 = vertex1;
		v2 = vertex2;
		v3 = vertex3;
	}
};



// globals
static int win;			// Window identifier
static int fps = 100;	// Frames per second
static int width = 500;
static int height = 500;
static char initTitle[] = "Color Cube";	// Initial title of the window
csc::Arduino *arduino = NULL;
bool calibrated = false;


// Function prototypes
void renderScene();
void updateScene(int delta);
void callTimer();
void keyEvents(unsigned char key, int x, int y);
void initGL();
void endAppRoutine();
void setupData();
void cleanupData();
void drawCube(float s);


// Application specific stuff
static float xRot = 0.0f;
static float yRot = 0.0f;
static float zRot = 0.0f;
static vector<color3f *> colors;
static vector<vertex3f *> vertices;
static vector<polygon3 *> polygons;


// Setups the data for us
void setupData() {
	// Colors used
	color3f *white = new color3f(1.0f, 1.0f, 1.0f);
	color3f *black =  new color3f(0.0f, 0.0f, 0.0f);
	color3f *red = new color3f(1.0f, 0.0f, 0.0f);
	color3f *yellow = new color3f(1.0f, 1.0f, 0.0f);
	color3f *green = new color3f(0.0f, 1.0f, 0.0f);
	color3f *cyan = new color3f(0.0f, 1.0f, 1.0f);
	color3f *blue = new color3f(0.0f, 0.0f, 1.0f);
	color3f *magenta = new color3f(1.0f, 0.0f, 1.0f);
	
	colors.push_back(white);
	colors.push_back(black);
	colors.push_back(red);
	colors.push_back(yellow);
	colors.push_back(green);
	colors.push_back(cyan);
	colors.push_back(blue);
	colors.push_back(magenta);
	
	// The vertices
	vertex3f *vWhite = new vertex3f(1.0f, 1.0f, -1.0f, white);
	vertex3f *vYellow = new vertex3f(1.0f, 1.0f, 1.0f, yellow);
	vertex3f *vRed = new vertex3f(-1.0f, 1.0f, 1.0f, red);
	vertex3f *vMagenta = new vertex3f(-1.0f, 1.0f, -1.0f, magenta);
	vertex3f *vCyan = new vertex3f(1.0f, -1.0f, -1.0f, cyan);
	vertex3f *vGreen = new vertex3f(1.0f, -1.0f, 1.0f, green);
	vertex3f *vBlack = new vertex3f(-1.0f, -1.0f, 1.0f, black);
	vertex3f *vBlue = new vertex3f(-1.0f, -1.0f, -1.0f, blue);
	
	vertices.push_back(vWhite);
	vertices.push_back(vBlack);
	vertices.push_back(vRed);
	vertices.push_back(vYellow);
	vertices.push_back(vGreen);
	vertices.push_back(vCyan);
	vertices.push_back(vBlue);
	vertices.push_back(vMagenta);
	
	// the polygons
	polygon3 *p1 = new polygon3(vWhite, vYellow, vRed);		// Top
	polygon3 *p2 = new polygon3(vWhite, vMagenta, vRed);	// Top
	polygon3 *p3 = new polygon3(vCyan, vGreen, vBlack);		// Bottom
	polygon3 *p4 = new polygon3(vCyan, vBlue, vBlack);		// Bottom
	polygon3 *p5 = new polygon3(vWhite, vCyan, vMagenta);	// Facing-us
	polygon3 *p6 = new polygon3(vBlue, vCyan, vMagenta);	// Facing-us
	polygon3 *p7 = new polygon3(vRed, vYellow, vBlack);		// Facing-away
	polygon3 *p8 = new polygon3(vGreen, vYellow, vBlack);	// Facing-away
	polygon3 *p9 = new polygon3(vGreen, vYellow, vCyan);	// Right
	polygon3 *p10 = new polygon3(vWhite, vYellow, vCyan);	// Right
	polygon3 *p11 = new polygon3(vBlue, vMagenta, vBlack);	// Left
	polygon3 *p12 = new polygon3(vRed, vMagenta, vBlack);	// Left
	
	polygons.push_back(p1);
	polygons.push_back(p2);
	polygons.push_back(p3);
	polygons.push_back(p4);
	polygons.push_back(p5);
	polygons.push_back(p6);
	polygons.push_back(p7);
	polygons.push_back(p8);
	polygons.push_back(p9);
	polygons.push_back(p10);
	polygons.push_back(p11);
	polygons.push_back(p12);
}


// Cleans up all the data that we have used
void cleanupData() {
	// Clear out the colors
	for (unsigned int i = 0; i < colors.size(); i++)
		delete colors[i];
	colors.clear();
	
	// Clear out the vertices
	for (unsigned int i = 0; i < vertices.size(); i++)
		delete vertices[i];
	vertices.clear();
	
	// Clear out the polygons, and we're done!
	for (unsigned int i = 0; i < polygons.size(); i++)
		delete polygons[i];
	polygons.clear();
}


// Draws a cube for us, with a scale
void drawCube(float s) {
	glBegin(GL_TRIANGLES);
		// Setup an iterator
		vector<polygon3 *>::iterator pIter;
		
		for (pIter = polygons.begin(); pIter < polygons.end(); pIter++) {
			// First vertex
			glColor3f((*pIter)->v1->clr->r,
					  (*pIter)->v1->clr->g,
					  (*pIter)->v1->clr->b);
			glVertex3f(s * (*pIter)->v1->x,
					   s * (*pIter)->v1->y,
					   s * (*pIter)->v1->z);
					   
			// Second vertex
			glColor3f((*pIter)->v2->clr->r,
					  (*pIter)->v2->clr->g,
					  (*pIter)->v2->clr->b);
			glVertex3f(s * (*pIter)->v2->x,
					   s * (*pIter)->v2->y,
					   s * (*pIter)->v2->z);
					   
			// Third vertex
			glColor3f((*pIter)->v3->clr->r,
					  (*pIter)->v3->clr->g,
					  (*pIter)->v3->clr->b);
			glVertex3f(s * (*pIter)->v3->x,
					   s * (*pIter)->v3->y,
					   s * (*pIter)->v3->z);
		}
	glEnd();
}


// Initilizes some stuff about OpenGL for us
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glTranslatef(0.0f, 0.0f, -6.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	
	glMatrixMode(GL_MODELVIEW);
}


// Renders the scene
void renderScene() {
	// Clear the scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Rotate the cube based upon gyro data
	/* NOTE: I'm not an expert on gyro/accelerometer data & algorithsm, so
	 *       it will not rotate perfectly.  If you are interested in some better
	 *       ones, check out someting called FreeIMU.
	 *
	 */
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 0.0f, -1.0f);
	glRotatef(zRot, 0.0f, 1.0f, 0.0f);
	
	// Draw the stuff
	drawCube(0.55f);
	
	// Swap the GLUT buffers
	glutSwapBuffers();
}


// Updates the scene
// NOTE: delta is currently unsued
void updateScene(int delta) {
	unsigned char buffer[15];
	int i = 0;
	static mpu_6050 mpu, calibration;


	// Hold it there till we get a response
	i = arduino->available();
	if (i == -1)
		perror("Error trying to see how many bytes were ready.\n");

	if (i < 15) {
		buffer[0] = REQ_ALL;
		arduino->send(buffer, 1);
		return;
	} else {
		arduino->recv(buffer, 15);
		bufferToStruct(&mpu, buffer);
	}

	if (mpu.error != 0)
		perror("DATA ERROR\n");
	else {
		// Do claibration
		if (!calibrated)  {
			// It's kind of, um, the "poor man's calibration."
			calibration.x_gyro += mpu.x_gyro;
			calibration.y_gyro += mpu.y_gyro;
			calibration.z_gyro += mpu.z_gyro;
			calibration.x_gyro /= 2;
			calibration.y_gyro /= 2;
			calibration.z_gyro /= 2;

			calibrated = true;
		}

		float scale = 11.0f;

		// Set the rotation values
		float percent = (float)(mpu.x_gyro - calibration.x_gyro);
		percent /= 32767.0f;
		xRot += scale * percent;

		percent = (float)(mpu.y_gyro - calibration.y_gyro);
		percent /= 32767.0f;
		yRot += scale * percent;

		percent = (float)(mpu.z_gyro - calibration.z_gyro);
		percent /= 32767.0f;
		zRot += scale * percent;
	}
}


// Is used to cap the application's frames
void callTimer() {
	int sleepFor = 1000000 / fps;
	
	usleep(sleepFor);		// Sleep for a bit, conserve CPU power
	updateScene(sleepFor);		// Update the scene
	glutPostRedisplay();		// Re-render it.
}


// A routine for ending the application
void endAppRoutine() {
	glutDestroyWindow(win);	// Clear the GLUT window
	cleanupData();			// Clean out all of our heap memory

	// SERIAL:
	delete arduino;

	exit(EXIT_SUCCESS);
}


// Registers keyboard events
void keyEvents(unsigned char key, int x, int y) {
	// Close the window, if the 'ESC' key was pressed
	if (key == 27)
		endAppRoutine();
	else if (key == 'r')
		xRot = yRot = zRot = 0.0f;
	else if (key == 'c')
		calibrated = false;
}


void int_signal(int sig) {
	printf("INTERUPTED!! (How Rude.)\n");
	endAppRoutine();
}


// Main function
int main(int argc, char *argv[]) {
	// Initilze glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	
	setupData();	// Set the data for the program

	// SERIAL:
	if (argc < 3) {
		printf("Usage:\n");
		printf("\t%s <port> <baudrate>\n", argv[0]);
		exit(1);
	}

	arduino = new csc::Arduino(argv[1], atoi(argv[2]));
	if (arduino->isConnectionSetup() == false) {
		fprintf(stderr, "Could not setup conneciton.\n");
		exit(1);
	}
	
	// connect the signal for an interrupt
	if (signal(SIGINT, int_signal) == SIG_ERR)
		exit(EXIT_FAILURE);

	if (arduino->startConnection() == -1)
		exit(1);
	
	// Create the window
	win = glutCreateWindow(initTitle);
	
	// Register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(callTimer);
	glutKeyboardFunc(keyEvents);
	
	// Initlize some things about OpenGL
	initGL();

	// call the main loop
	glutMainLoop();

	return 0;
}

