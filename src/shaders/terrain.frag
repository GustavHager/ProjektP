#version 150

in vec2 texCoord;
in vec3 exNormal;
in vec3 viewDir;

out vec4 outColor;


uniform sampler2D tex;

void main(void)
{
	float intensity = 3.0;
	float diffuse;
	float specular;
    vec3 lightVector = vec3(0,1.0,0.7);
    vec3 s;
    vec3 r;
    float ratio = 0.5;
    float n = 2;
    vec3 lightColor = vec3(1,1,1);
    vec3 light;

    diffuse = intensity * dot(exNormal,lightVector);

    s = lightVector;
    r = 2*exNormal*dot(s,exNormal) - s;

    specular = intensity*pow(dot(r,viewDir),n);

    light = ratio*max(0,diffuse)* lightColor;

    outColor = normalize(texture(tex, texCoord) * vec4(light,1));





}
