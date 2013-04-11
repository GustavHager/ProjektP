#ifndef __terrain__
#define __terrain__

#include <GL/gl.h>
#include <math.h>
#include "libs/VectorUtils2.h"
#include "libs/GL_utilities.h"
#include "libs/loadobj.h"
#include "libs/LoadTGA2.h"

GLfloat *vertexArray;
int worldWidth;
int worldHeight;

Point3D* getQuad(GLfloat x, GLfloat z);
GLfloat getHeightInPoint(GLfloat x,GLfloat z);
Model* GenerateTerrain(TextureData *tex);

#endif
