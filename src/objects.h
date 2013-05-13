#ifndef __objects__
#define __objects__

#include <GL/gl.h>
#include <math.h>
#include "libs/VectorUtils2.h"
#include "libs/GL_utilities.h"
#include "libs/loadobj.h"
#include "libs/LoadTGA2.h"
#include "libs/objectHandler.h"

static objectData skybox;

void drawSkybox(GLfloat *lookatMatrix, GLfloat* projMatrix, GLfloat* mdlMatrix);
void loadSky(GLuint shaderProgram);
#endif
