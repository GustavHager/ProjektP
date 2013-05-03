#ifndef __camera__
#define __camera__

#include <GL/gl.h>
#include <math.h>
#include "libs/VectorUtils2.h"
#include "libs/GL_utilities.h"
#include "libs/loadobj.h"
#include "libs/LoadTGA2.h"
#include "libs/objectHandler.h"
#include "terrain.h"
#include "objects.h"

#define PI 3.1415

//camera position
static Point3D p = {.x = 0, .y = 0, .z = 8};
//look at point
static Point3D l = {.x = 0, .y = -1, .z = 0};

static Point3D viewDirection;
static Point3D leftRight;
static Point3D toTheSky = {0, 1, 0};

static GLfloat yOffset = 1.0;

static GLfloat radius = 10;

static GLfloat windowWidth = 800;
static GLfloat windowHight = 800;

// vertex array object
Model *m, *m2, *tm;

GLfloat projectionMatrix[16];

// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void timer(int i);
void mouse(int x, int y);
void display(void);
void init();

#endif
