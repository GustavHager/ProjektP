#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 exNormal;
out vec4 colorMod;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform vec2 heightRange;


void main(void){

	mat3 normalMatrix = mat3(mdlMatrix);

	exNormal = inNormal * normalMatrix;
	texCoord = inTexCoord;

    //vec4 white = vec4(1,1,1,1);
    //vec4 green = vec4(0,1,0,1);


	colorMod = vec4(1.0,
                    1.0,
                    1.0,
                    1.0);


	gl_Position = projMatrix * mdlMatrix * camMatrix * vec4(inPosition, 1.0);
}
