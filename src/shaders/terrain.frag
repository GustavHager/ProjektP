#version 150

out vec4 outColor;
uniform sampler2D tex;

in vec2 texCoord;
in vec3 fragNormal;
in vec3 viewDir;

void main(void)
{
    float intensity = 2;
	float diffuse;
	float specular;
    vec3 lightVector = vec3(1,0.5,0);
    vec3 s;
    vec3 r;
    float ratio = 0.7;
    float n = 2;
    vec3 lightColor = vec3(1,1,1);
    vec3 light;

    diffuse = intensity * dot(fragNormal,lightVector);
    s = lightVector;
    r = 2*fragNormal*dot(s,fragNormal) - s;
    specular = intensity*pow(dot(r,viewDir),n);
    light = ratio*max(0,diffuse) * lightColor+(1-ratio)*max(0,specular)*lightColor;
    outColor = normalize(texture(tex, texCoord) * vec4(light,1));
}
