#include "camera.h"

void timer(int i)
{

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



    //GLfloat h = getHeightInPoint(p.x,p.z);

    p.y = yOffset;
    l.y = yOffset;


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

void mouse(int x, int y){
    p.x = l.x + radius*cos(2*PI*x/windowWidth);

	p.z = l.z + radius*sin(2*PI*x/windowWidth);
}

void display(void){
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat total[16], modelView[16], camMatrix[16], transMatrix[16], newTotal[16];

	printError("pre display");

	// Build matrix
	lookAt(&p,&l,0, 1, 0,camMatrix);

	IdentityMatrix(modelView);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView);
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);

  	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	
	
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	printError("display 4");


	glutSwapBuffers();
}

void init(void){
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

	frustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0, projectionMatrix);


	// Load and compile shader
	program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("res/dirt.tga", &tex1);


	//Load terrain data

	LoadTGATexture("res/TERRA2.tga", &ttex);
	tm = GenerateTerrain();
	printError("init terrain");

}
