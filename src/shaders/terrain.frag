#version 150

in vec2 texCoord;
in vec3 exNormal;
in vec4 colorMod;

out vec4 outColor;


uniform sampler2D tex;

void main(void)
{
	const vec3 light = vec3(0.0,1.0,0.0);
	const vec4 white = vec4(1.0,1.0,1.0,1.0);
	const vec4 green = vec4(0.0,0.5,0.0,1.0);
	const vec4 gray  = vec4(0.5,0.5,0.5,1.0);

	vec4 color;

	float shade = dot(normalize(exNormal),light);
	float slope = dot(normalize(exNormal),vec3(0.0,1.0,0.0));

    color = gray*(1 - slope) + green * slope;

    //outColor = color * shade;

    outColor = texture(tex, texCoord) * shade;
}
