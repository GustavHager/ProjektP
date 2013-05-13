#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 exNormal;
out vec3 viewDir;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;


void main(void){

	mat3 normalMatrix = mat3(mdlMatrix);

	exNormal = normalize(inNormal);

	texCoord = inTexCoord;
    vec3 eyeToSurface = vec3(mdlMatrix* vec4(inPosition,1));
    viewDir = normalize(eyeToSurface);

	gl_Position = projMatrix * mdlMatrix * camMatrix * vec4(inPosition, 1.0);
}
