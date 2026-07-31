#version 450 core

layout (location = 0) in vec3 aPos;         // position variable has attribute position 0
layout (location = 1) in vec4 aColor;       // color variable has attribute position 1
layout (location = 2) in vec3 aNormal;      // normal variable has attribute position 2
layout (location = 3) in vec2 coord_st;     // texture variable has attribute position 3

in vec2 vTexCoord;
  
out vec4 ourColor;      // output a color to the fragment shader


out vec3 vcsN;
out vec3 vcsPosition;
out vec3 vcsLightPosition;
out vec2 tex_coord_st;

uniform mat4 Projection;  
uniform mat4 Model;
uniform mat4 View;
uniform vec3 ViewPos;
uniform float time;

//Struttura per la gestione di un punto luce
struct PointLight {
	vec3 position;
	vec3 color;
	float power;
};
//definizione di una variabile uniform che ha la struttura PointLight
uniform PointLight light;

//Struttura per la gestione di un materiale
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

uniform int sceltaShader;

float strenght = 0.1;

void main()
{
    ourColor = aColor;    // set ourColor to the input color we got from the vertex data

    if (sceltaShader == 0)                      // NONE  v_passthrough
        gl_Position = Projection * View * Model * vec4(aPos, 1.0);
    
    if (sceltaShader == 1 )                    // GOURAUD_SHADING 
    {
        gl_Position = Projection * View * Model * vec4(aPos, 1.0);

        // Transform vertex position into VCS coordinates
        vec4 vcsPosition = View * Model * vec4(aPos, 1.0);
        // Transform Light  position into VCS coordinates 
	    vec4 vcsLightPos = View * vec4(light.position, 1.0);
        
        // Compute vectors N,V,L,R in VCS
        vec3 N = normalize(transpose(inverse(mat3(View * Model))) * aNormal);     
        vec3 V = normalize(ViewPos - vcsPosition.xyz);
        vec3 L = normalize((vcsLightPos - vcsPosition).xyz);
        vec3 R = reflect(-L, N);  //Costruisce la direzione riflessa di L rispesso alla normale

        //ambient
        vec3 ambient = strenght * light.power * material.ambient;

        //diffuse
        float cos_angolo_theta = max(dot(L, N), 0);
        vec3 diffuse = light.power * light.color * cos_angolo_theta * material.diffuse;

        //specular
        float spec = pow(max(dot(V, R), 0), material.shininess);
        vec3 specular = light.power * light.color *  spec * material.specular;

        ourColor = vec4(ambient + diffuse + specular, 1.0);
        tex_coord_st = coord_st;
    }

    if (sceltaShader == 2  || sceltaShader == 3 ) // PHONG_SHADING o TEX_PHONG_SHADING
    {
        gl_Position = Projection * View * Model * vec4(aPos, 1.0);

	    // Transform Vertex position into VCS coordinates
        vcsPosition = vec3(View * Model * vec4(aPos, 1.0));
        
        // Transform Light  position into VCS coordinates 
	    vcsLightPosition = vec3(View * vec4(light.position, 1.0));
        
        // Compute normal vector in VCS
        vcsN = (transpose(inverse(mat3(View * Model))) * aNormal);
        tex_coord_st = coord_st;
    }

    if (sceltaShader == 5) // Wave
    {
        vec4 v = vec4(aPos, 1);
        v.y = (sin(80.0 * v.x + time) * cos(8.5 * v.y + time) * 0.1);
        gl_Position = Projection * View * Model * v;
        tex_coord_st = coord_st;
    }

    if (sceltaShader == 6) // Animazione Bandiera
    {
        vec4 v = vec4(aPos, 1);
        float offset = 0.3 * sin(0.5 * time + 5.0 * v.x);
        v.z = v.z + offset;
        gl_Position = Projection * View * Model * v;
        tex_coord_st = coord_st;
    }

    if (sceltaShader == 4) // only_texture
    {
        gl_Position = Projection * View * Model * vec4(aPos,1.0);
        tex_coord_st = coord_st;
    }

}  

