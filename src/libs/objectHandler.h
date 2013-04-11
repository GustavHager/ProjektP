#ifndef objectHandler
#define objectHandler

#include "loadobj.h"
#include "VectorUtils2.h"

typedef struct objectData{
    Model *m;
    GLfloat x,y,z;
    GLfloat dx,dy,dz;
    GLfloat rotationMatrix[16];
    GLuint shaderProgram;
    GLuint texture0,texture1;
} objectData;

//load object from file, with or without texture
objectData loadObject(char *modelFilename);
//load object from file, with shader and texture
objectData loadObjectWithMore(char *modelFilename, GLuint shaderProgram, GLuint texture);
//get translation matrix for object
void getT(objectData *o, GLfloat *matrix);

//draw the object
void drawObject(objectData *o);

#endif // objectHandler
