#version 150

uniform mat4 projMatrix;
uniform mat4 lookatMatrix;
uniform mat4 transMatrix;
uniform mat4 rotMatrix;

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;

void main(void)
{

	//fragNormal = normalize( inNormal);
	//fragTexCoord = inTexCoord;

	gl_Position = projMatrix * lookatMatrix * transMatrix * rotMatrix * vec4(inPosition,1);
}
