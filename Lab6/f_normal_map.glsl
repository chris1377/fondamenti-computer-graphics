// Fragment shader: : Normal mapping shading
// ================
#version 450 core

in vec2 _TexCoord;
in vec3 TangentLightPos;  // L vector
in vec3 TangentViewPos;   // E vector
in vec3 TangentFragPos;   // pos

// Ouput data
out vec4 FragColor;

//uniforms
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct PointLight{
	vec3 position;
	vec3 color;
	float power;
 };
uniform PointLight light;

#define linear_coef 0.01
#define quadratic_coef 0.01

void main()
{    
   // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, _TexCoord).rgb;
   
   // transform normal vector to range [-1,1]
   normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
  
   // get diffuse color
   vec3 color = texture(diffuseMap, _TexCoord).rgb;
   
   // ambient
   vec3 ambient = 0.01 * color;

   // diffuse
   vec3 lightDir = TangentLightPos - TangentFragPos;
   //attenuation
   //float distance = length(lightDir);
    //https://www.desmos.com/calculator/nmnaud1hrw try to play with the attenuation function
   //float attenuation = 1.0 / ( 1.0 + linear_coef * distance  +  quadratic_coef * (distance*distance));

   lightDir = normalize(lightDir);
   float diff = max(dot(normal, lightDir), 0.0) ;
   vec3 diffuse =  light.power * light.color * diff * color ; //* attenuation;
 
   // specular
   vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
   vec3 reflectDir = reflect(-lightDir, normal); 
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) ;
   vec3 specular = light.power * light.color * material.specular * spec ; //* attenuation;
   
   FragColor = vec4(ambient + diffuse + specular, 1.0);
}