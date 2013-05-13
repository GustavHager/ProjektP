#include "objectHandler.h"


objectData loadObjectWithMore(char *modelFilename, GLuint shaderProgram, GLuint texture){
    objectData o = loadObject(modelFilename);
    o.shaderProgram = shaderProgram;
    o.texture0 = texture;
    return o;
}

objectData loadObject(char *modelFilename){
    Model *m = LoadModelPlus(modelFilename);

    objectData d;
    d.m = m;
    d.x = d.y = d.z = d.dx = d.dy = d.dz = 0;
    d.texture0 = d.texture1 = 0;
    T(0,0,0,&d.rotationMatrix);

    return d;
}

void getT(objectData *o, GLfloat *matrix){
    T(o->x,o->y,o->z,matrix);
}

void drawObject(objectData *o){
    static GLfloat tMatrix[16];

    glUseProgram(o->shaderProgram);

    getT(o,tMatrix);

    glUniformMatrix4fv(glGetUniformLocation(o->shaderProgram, "transMatrix"), 1, GL_TRUE, tMatrix);
    glUniformMatrix4fv(glGetUniformLocation(o->shaderProgram, "rotMatrix"), 1, GL_TRUE, o->rotationMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,o->texture0);

    glUniform1i(glGetUniformLocation(o->shaderProgram,"tex0"), 0); //Texture unit 0

    if(o->texture1 != 0){
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,o->texture1);
        glUniform1i(glGetUniformLocation(o->shaderProgram,"tex1"), 1); //tex unit 1
    }else{
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,o->texture0);
        glUniform1i(glGetUniformLocation(o->shaderProgram,"tex1"), 0); //tex unit 1
    }

    int h = glGetUniformLocation(o->shaderProgram, "rotMatrix");


    DrawModel(o->m,o->shaderProgram,"inPosition","inNormal","inTexCoord");

}
