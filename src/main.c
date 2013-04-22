#include <stdio.h>
#include <math.h>

#include "libs/GL_utilities.h"
#include "libs/VectorUtils2.h"
#include "libs/loadobj.h"
#include "libs/LoadTGA2.h"
#include "libs/objectHandler.h"

#include "terrain.h"
#include "camera.h"
#include "objects.h"

#define PI 3.1415

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (windowWidth, windowHight);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init();
	initKeymapManager();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
