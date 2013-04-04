#include <stdio.h>
#include <math.h>
#include "libs/GL_utilities.h"
#include "libs/VectorUtils2.h"
#include "libs/loadobj.h"
#include "libs/LoadTGA2.h"
#include "libs/objectHandler.h"


#define PI 3.1415

GLfloat projectionMatrix[16];


//camera position
Point3D p = {.x = 0, .y = 0, .z = 8};
//look at point
Point3D l = {.x = 0, .y = -1, .z = 0};

Point3D viewDirection;

Point3D leftRight;
Point3D toTheSky = {0, 1, 0};

//camera 'up'
GLfloat vx = 0;
GLfloat vy = 1;
GLfloat vz = 0;

GLfloat yOffset = 1.0;

GLfloat radius = 10;

GLfloat windowWidth = 800;
GLfloat windowHight = 800;

GLfloat *vertexArray;
int worldWidth;
int worldHeight;


GLfloat lookatMatrix[] = {1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1};

objectData ball;

objectData loadBalls(){

    //load shader
    GLuint ballShader = loadShaders("ball.vert", "ball.frag");
    GLuint ballTexture;

    glUseProgram(ballShader);
    LoadTGATextureSimple("f3.tga",&ballTexture);
    ball = loadObjectWithMore("groundsphere.obj",ballShader,ballTexture);
    //glUniformMatrix4fv(glGetUniformLocation(ball.shaderProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix);
    printError("ballsproblem.");
}



Point3D* getQuad(GLfloat x, GLfloat z){

Point3D *corners;
corners = malloc(sizeof(Point3D)*4);

int bottomX,bottomZ,topX,topZ;

bottomX = floor(x);
bottomZ = floor(z);

topX = ceil(x);
topZ = ceil(z);

if ((x <= worldWidth) && (z <= worldHeight) && (z >= 0) && (x >= 0)){
    //first corner
    corners[0].x = vertexArray[(bottomX + bottomZ * worldWidth)*3+0];
    corners[0].y = vertexArray[(bottomX + bottomZ * worldWidth)*3+1];
    corners[0].z = vertexArray[(bottomX + bottomZ * worldWidth)*3+2];

    //second corner
    corners[1].x = vertexArray[(bottomX + topZ * worldWidth)*3+0];
    corners[1].y = vertexArray[(bottomX + topZ * worldWidth)*3+1];
    corners[1].z = vertexArray[(bottomX + topZ * worldWidth)*3+2];

    //third corner
    corners[2].x = vertexArray[(topX + topZ * worldWidth)*3+0];
    corners[2].y = vertexArray[(topX + topZ * worldWidth)*3+1];
    corners[2].z = vertexArray[(topX + topZ * worldWidth)*3+2];

    //fourth corner
    corners[3].x = vertexArray[(topX + bottomZ * worldWidth)*3+0];
    corners[3].y = vertexArray[(topX + bottomZ * worldWidth)*3+1];
    corners[3].z = vertexArray[(topX + bottomZ * worldWidth)*3+2];

    //printf("first corner in:(%f,%f,%f)\n",corners[0].x,corners[0].y,corners[0].z);
	}else{
	    //printf("worldX: %i, worldZ: %i, you outside!\n",worldHeight,worldWidth);

	    int i=0;
        for(i=0; i < 4; i++){
            corners[i].x = corners[i].y = corners[i].z = 0;
        }
	}
    return corners;
}

GLfloat getHeightInPoint(GLfloat x,GLfloat z){

    GLfloat height;
    Point3D *quadCorners;
    quadCorners = getQuad(x,z);

    Point3D diag,pVector,crossResult;

    Point3D currPoint = {.x = x, .y=0, .z=z};

    VectorSub(&quadCorners[2],&quadCorners[0],&diag);
    VectorSub(&currPoint,&quadCorners[0],&pVector);


    CrossProduct(&diag, &pVector, &crossResult);
    if(crossResult.y > 0){
        height = (quadCorners[0].y + quadCorners[1].y + quadCorners[2].y) / 3;
    }else{
        height = (quadCorners[0].y + quadCorners[3].y + quadCorners[2].y) / 3;
    }

    //free(quadCorners);
    return height;
}

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	worldWidth = tex->width;
	worldHeight = tex->height;

	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	GLfloat Xcoord,Ycoord,Zcoord,X2coord,Y2coord,Z2coord,X3coord,Y3coord,Z3coord;

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.

            if(x > 0 && z > 0) {
                Xcoord = vertexArray[(x + z * tex->width)*3 + 0];
                Ycoord = vertexArray[(x + z * tex->width)*3 + 1];
                Zcoord = vertexArray[(x + z * tex->width)*3 + 2];

                X2coord = vertexArray[((x - 1)+ z * tex->width)*3 + 0];
                Y2coord = vertexArray[((x - 1)+ z * tex->width)*3 + 1];
                Z2coord = vertexArray[((x - 1)+ z * tex->width)*3 + 2];

                X3coord = vertexArray[((x + 1)+ z * tex->width)*3 + 0];
                Y3coord = vertexArray[((x + 1)+ z * tex->width)*3 + 1];
                Z3coord = vertexArray[((x + 1)+ z * tex->width)*3 + 2];

                Point3D v1 = {.x=Xcoord - X2coord, .y=Ycoord - Y2coord, .z=Zcoord - Z2coord};
                Point3D v2 = {.x=X2coord - X3coord, .y=Y2coord - Y3coord, .z=Z2coord - Z3coord};
                Point3D normalVector;

                CrossProduct(&v2,&v1,&normalVector);

                normalArray[(x + z * tex->width)*3 + 0] = normalVector.x;
                normalArray[(x + z * tex->width)*3 + 1] = normalVector.y;
                normalArray[(x + z * tex->width)*3 + 2] = normalVector.z;

            }else{
                normalArray[(x + z * tex->width)*3 + 0] = 0.0;
                normalArray[(x + z * tex->width)*3 + 1] = 1.0;
                normalArray[(x + z * tex->width)*3 + 2] = 0.0;
            }

// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] =  (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] =  (float)z / tex->height;
		}

	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0, projectionMatrix);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("dirt.tga", &tex1);


// Load terrain data

	LoadTGATexture("TERRA2.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	loadBalls();
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat total[16], modelView[16], camMatrix[16], transMatrix[16], newTotal[16];


	printError("pre display");

	glUseProgram(program);

	// Build matrix

	lookAt(&p,&l,0, 1, 0,camMatrix);

	IdentityMatrix(modelView);
	Mult(camMatrix, modelView, total);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMatrix"), 1, GL_TRUE, camMatrix);
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");

    glUseProgram(ball.shaderProgram);
    //upload matrices to the balls shader
    glUniformMatrix4fv(glGetUniformLocation(ball.shaderProgram, "lookAtMatrix"), 1, GL_TRUE, camMatrix);
    glUniformMatrix4fv(glGetUniformLocation(ball.shaderProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix);

    ball.shaderProgram = program;

    //compute new modulating matrix
    //translation matrix
    T(ball.x,ball.y,ball.z,&transMatrix);
    Mult(total,transMatrix,&newTotal);

    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, newTotal);

    //draw the ball
    drawObject(&ball);

	glutSwapBuffers();
}

void timer(int i)
{

    if(keyIsDown('t')){
        ball.x = ball.x + 1;
        ball.y = ball.y + 1;
        printf("ball is at:%f",ball.x);
    }

    if(keyIsDown('y')){
        ball.x = ball.x - 1;
        ball.y = ball.y - 1;
    }

    VectorSub(&l,&p,&viewDirection);
    Normalize(&viewDirection);
    viewDirection.y = 0;

    CrossProduct(&viewDirection,&toTheSky,&leftRight);
    Normalize(&leftRight);


    if(keyIsDown('s')){
        VectorSub(&p,&viewDirection,&p);
        VectorSub(&l,&viewDirection,&l);
    }


    if(keyIsDown('w')){
        //p.z = p.z - 0.5;
        //l.z = l.z - 0.5;

        VectorAdd(&p,&viewDirection,&p);
        VectorAdd(&l,&viewDirection,&l);
    }


    if(keyIsDown('a')){
        VectorSub(&p,&leftRight,&p);
        VectorSub(&l,&leftRight,&l);
    }

    if(keyIsDown('d')){
        VectorAdd(&p,&leftRight,&p);
        VectorAdd(&l,&leftRight,&l);
    }


    Point3D offsetVector;

    if(keyIsDown('q')){
        yOffset = yOffset-0.2;
    }

    if(keyIsDown('e')){
       yOffset = yOffset+0.2;
    }

    if(keyIsDown('h')){
        ball.x = ball.x + 0.1;
    }

    if(keyIsDown('j')){
        ball.z = ball.z + 0.1;
    }

    if(keyIsDown('k')){
        ball.x = ball.x - 0.1;
    }

    if(keyIsDown('l')){
        ball.z = ball.z - 0.1;
    }

    ball.y = getHeightInPoint(ball.x,ball.z);

    GLfloat h = getHeightInPoint(p.x,p.z);

    //printf("x: %f  ",p.x);
    //printf("z: %f  ",p.z);
    //printf("h: %f \n", h);

    p.y = yOffset + h;
    l.y = yOffset + h;


    if(keyIsDown('z')){
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    if(keyIsDown('x')){
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }



    glutPostRedisplay();

	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int x, int y)
{
    p.x = l.x + radius*cos(2*PI*x/windowWidth);
	//p.y = l.y + radius*sin(2*PI*y/windowHight);
	p.z = l.z + radius*sin(2*PI*x/windowWidth);


}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (windowWidth, windowHight);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
