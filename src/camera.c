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
        l.y = l.y - 1;
        p.y = p.y - 1;
    }

    if(keyIsDown('e')){
       l.y = l.y + 1;
       p.y = p.y + 1;
    }



    //GLfloat h = getHeightInPoint(p.x,p.z);

    //p.y = yOffset;
    //l.y = yOffset;


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
	IdentityMatrix(modelView);
	lookAt(&p,&l,0, 1, 0,camMatrix);


    //drawSkybox(lookAt,projectionMatrix,modelView);
    glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ttex.texID);		// Bind Our Texture tex1


	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	printError("display 4");

	glutSwapBuffers();
}

void init(void){
	// GL inits
	glClearColor(0.5,0.81,1,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

	frustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000000.0, projectionMatrix);


	// Load and compile shader
	program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	printError("load shaders");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("res/dirt.tga", &tex1);


	GLfloat HeightRange[2] = {(double)Ymin, (double)Ymax};
	glUniform2fv(glGetUniformLocation(program, "heightRange"), 2, HeightRange);


	//Load terrain data
	//LoadTGATexture("res/TERRA2.tga", &ttex);
	tm = GenerateTerrain();
    ttex = generateColormap();
    printError("init terrain");

    //skybox.shaderProgram = loadShaders("shaders/sky.vert","shaders/sky.frag");
    //loadSky(skybox.shaderProgram);
    printError("skybox init");
    setCameraPosition(worldWidth/2,get_height(worldWidth/2,worldWidth/2,worldHeight,worldHeight)+10,worldWidth/2);
}

void setCameraPosition(float x, float y, float z){
    p.x = x;
    p.y = y;
    p.z = z;
    printf("i is : %f \n",y);
    l.x = x + 10;
    l.y = y;
    l.z = z + 10;
}
