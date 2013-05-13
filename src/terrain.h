#ifndef __terrain__
#define __terrain__

#include <GL/gl.h>
#include <math.h>
#include <time.h>
#include "libs/VectorUtils2.h"
#include "libs/GL_utilities.h"
#include "libs/loadobj.h"
#include "libs/LoadTGA2.h"

struct square{
	int x,z;
	int w,h;
	struct square *next;
};

struct square *s_stack;

GLfloat *vertexArray;
int worldWidth;
int worldHeight;
double Ymin;
double Ymax;
GLfloat* heightmap;

GLubyte* colormap;

GLfloat max_point;
GLfloat water_level;
GLfloat min_point;
GLfloat average_height;
GLfloat max_slope;


Point3D* getQuad(GLfloat x, GLfloat z);
GLfloat getHeightInPoint(GLfloat x,GLfloat z);
Model* GenerateTerrain();

void generate_world(int width, int height);
void initWorldgen(int width, int height);
GLfloat get_height(int x, int z, int width, int height);
void displace_terrain(int width, int height,double displacement);
int get_index(int x, int z);

void post_process_terrain();

TextureData generateColormap(void);

GLfloat waterLevel;
GLfloat sandLevel;
GLfloat grassLevel;
GLfloat snowLevel;


#endif
