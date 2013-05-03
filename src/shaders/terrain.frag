#version 150

in vec2 texCoord;
in vec3 exNormal;
in vec4 colorMod;

out vec4 outColor;


uniform sampler2D tex;

void main(void)
{
	const vec3 light = vec3(0.5,1.0,0.5);

    vec4 color = vec4(1,0,0,0);

	float shade = dot(normalize(exNormal),light);


	//outColor = texture(tex, texCoord) * shade;
	outColor = colorMod * shade;
}
