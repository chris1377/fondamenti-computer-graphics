#version 450 core

in  vec4 ourColor ;








in vec3 vcsN;
in vec3 vcsPosition;
in vec3 vcsLightPosition;

in float waveEdge;

out vec4 FragColor;  

uniform int sceltaShader; 

struct PointLight {
	vec3 position;
	vec3 color;
	float power;
};
uniform PointLight light;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

float strenght = 0.2;

void main(){
{
if (sceltaShader == 0 || sceltaShader == 1 || sceltaShader == 4 )   // NONE or GOURAUD_SHADING or NO_TEXTURE
    FragColor = ourColor;
}

if (sceltaShader == 3 ){ //PHONG

    vec3 ambient = strenght * light.power * material.ambient;

    vec3 norm = normalize(vcsN);
    vec3 lightDir = normalize(vcsLightPosition - vcsPosition);
    float cos_angolo_theta = max(dot(norm,lightDir),0.0);
    vec3 diffuse = light.power * light.color * cos_angolo_theta *material.diffuse;

    vec3 viewDir = normalize(-vcsPosition);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir), 0.0),material.shininess);
    vec3 specular = light.power * light.color * material.specular * spec;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}

if (sceltaShader == 5){ //wawe
    FragColor = vec4(0.0,0.0,0.5,1.0) + waveEdge*0.5;
}

if (sceltaShader == 6){ //toon

    vec4 a = vec4(0.0,0.5,0.8,1.0);
    vec4 b = vec4(0.0,0.3,0.6,1.0);
    vec4 c = vec4(0.0,0.2,0.5,1.0);
    vec4 d = vec4(0.0,0.3,0.6,1.0);
    vec4 e = vec4(0.0,0.0,0.1,1.0);

    vec4 color;
    float intensity = dot(normalize(vcsLightPosition),normalize(vcsN));
    if (intensity > 0.95)
        color = a;
    else if (intensity > 0.5)
        color = b;
    else if (intensity > 0.25)
        color = c;
    else if (intensity > 0.15)
        color = d;
    else
        color = e;

    float aa = dot(normalize(-vcsPosition), normalize(vcsN));
    if(aa >= 0.0 && aa < 0.30)
        color = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = color;
}




}