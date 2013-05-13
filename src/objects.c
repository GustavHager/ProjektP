#include "objects.h"


void drawSkybox(GLfloat* lookatMatrix, GLfloat* projMatrix, GLfloat* mdlMatrix) {
    printError("drawing skybox");
    glUseProgram(skybox.shaderProgram);
	glDisable(GL_DEPTH_TEST);
	GLfloat skyLookatMatrix[16];


	CopyMatrix(lookatMatrix,skyLookatMatrix);
	skyLookatMatrix[3] = 0;
	skyLookatMatrix[7] = -0.5;
	skyLookatMatrix[11] = 0;

    glUniformMatrix4fv(glGetUniformLocation(skybox.shaderProgram,"mdlMatrix"), 1, GL_TRUE, mdlMatrix);
    glUniformMatrix4fv(glGetUniformLocation(skybox.shaderProgram,"projMatrix"), 1, GL_TRUE, projMatrix);
	glUniformMatrix4fv(glGetUniformLocation(skybox.shaderProgram, "camMatrix"), 1, GL_TRUE, skyLookatMatrix);
    printError("skybox lookat upload");

    drawObject(&skybox);

    printError("skybox drawing");
    glUniformMatrix4fv(glGetUniformLocation(skybox.shaderProgram, "camMatrix"), 1, GL_TRUE, lookatMatrix); //restore the lookat matrix in the shader
	glEnable(GL_DEPTH_TEST);
}


void loadSky(GLuint shaderProgram){

//get a skybox
skybox = loadObject("res/skybox.obj");
skybox.shaderProgram = shaderProgram;
LoadTGATextureSimple("res/SkyBox512.tga",&skybox.texture0);

}
