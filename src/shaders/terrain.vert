#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 fragNormal;
out vec3 viewDir;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 lookAtMatrix;

void main(void)
{

	mat3 normalMatrix1 = mat3(mdlMatrix);
	fragNormal = vec3(normalize(inNormal));
	texCoord = inTexCoord;

	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);

    vec3 eyeToSurface = vec3(mdlMatrix* vec4(inPosition,1));
    viewDir = normalize(eyeToSurface);
}
