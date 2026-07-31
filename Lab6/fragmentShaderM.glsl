#version 450 core

in vec4     ourColor ;
// coordinate della texture del frammento corrente
in vec2 tex_coord_st; 




in vec3 vcsN;
in vec3 vcsPosition;
in vec3 vcsLightPosition;

out vec4    FragColor;  

//Dichiara un campionatore uniforme id_tex di tipo sampler2D. 
//Questo campionatore viene utilizzato per accedere a una texture 2D.
uniform sampler2D id_tex1;
uniform sampler2D id_tex2;
uniform sampler2D textureBuffer;

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

float strenght = 0.8;

void main()
{
if  (sceltaShader == 0 )   // NONE 
    FragColor = ourColor;

if  (sceltaShader == 1)                         // GOURAUD_SHADING
    FragColor = ourColor;

if (sceltaShader == 2){ //TEX_PHONG_SHADING

    vec3 texColor = texture(id_tex1, tex_coord_st).rgb;

    vec3 ambient = strenght * light.power * material.ambient;

    // diffuse 
    vec3 norm = normalize(vcsN);
	vec3 lightDir = normalize(vcsLightPosition - vcsPosition);
    float cos_angolo_theta = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.power * light.color * cos_angolo_theta *vec3(texture(id_tex1, tex_coord_st).rgb);
     
    // specular
    vec3 viewDir = normalize(-vcsPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  light.power * light.color * material.specular * spec;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

}


if (sceltaShader == 3)                        // PHONG_SHADING
    {    
    // ambient
	vec3 ambient = strenght * light.power * material.ambient;
     
    // diffuse 
    vec3 norm = normalize(vcsN);
	vec3 lightDir = normalize(vcsLightPosition - vcsPosition);
    float cos_angolo_theta = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.power * light.color * cos_angolo_theta *material.diffuse;
     
    // specular
    vec3 viewDir = normalize(-vcsPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  light.power * light.color * material.specular * spec;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
    }
    
if  (sceltaShader == 5 || sceltaShader == 6 || sceltaShader == 4) {    // TEXTURE_NO_LIGHT ONLY TEXTURE    
        FragColor = texture(id_tex1,tex_coord_st); 
        

}

       
      

}