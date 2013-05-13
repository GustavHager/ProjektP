#version 150

in vec2 texCoord;
in vec3 exNormal;

out vec4 outColor;


uniform sampler2D tex0;
uniform sampler2D tex1;

void main(void)
{

outColor = normalize(texture(tex0,texCoord));

}
