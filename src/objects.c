#include "objects.h"

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

void updateBall(){
    if(keyIsDown('t')){
        ball.x = ball.x + 1;
        ball.y = ball.y + 1;
        printf("ball is at:%f",ball.x);
    }

    if(keyIsDown('y')){
        ball.x = ball.x - 1;
        ball.y = ball.y - 1;
    }
}
