#version 150

in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 out_color;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main(void)
{

    //float ratio = sin(dot(fragTexCoord,fragTexCoord)*0.05);

    out_color = vec4(1,1,1,1);
}
