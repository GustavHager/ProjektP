#version 150

in vec2 texCoord;
in vec3 exNormal;

out vec4 outColor;

uniform sampler2D tex;

void main(void)
{
	const vec3 light = vec3(0.0,1.0,0.0);


	float shade = dot(normalize(exNormal),light);
	outColor = texture(tex, texCoord) * shade;
}
