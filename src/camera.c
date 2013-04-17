#include "camera.h"

void timer(int i)
{
    updateBall();

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

void mouse(int x, int y){
    p.x = l.x + radius*cos(2*PI*x/windowWidth);

	p.z = l.z + radius*sin(2*PI*x/windowWidth);
}

void display(void){
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

void init(void){
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0, projectionMatrix);

	// Load and compile shader
	program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("res/dirt.tga", &tex1);


// Load terrain data

	LoadTGATexture("res/TERRA2.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	loadBalls();
}
