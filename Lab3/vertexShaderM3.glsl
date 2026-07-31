#version 450 core
// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;         // the position variable has attribute position 0
layout (location = 1) in vec4 aColor;       // the color variable has attribute position 1
layout (location = 2) in vec3 aNormal;      // the normal to the vertex has attribute position 2
 



 
out vec4 ourColor;                          // output a color to the fragment shader

out vec3 vcsN;
out vec3 vcsPosition;
out vec3 vcsLightPosition;

out float waveEdge;

uniform mat4 Projection;  
uniform mat4 Model;
uniform mat4 View;

uniform vec3  ViewPos;
uniform float time;

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

uniform int sceltaShader;

float strenght = 0.2;

void main()
{
    ourColor = aColor;    // set ourColor to the input color we got from the vertex data

    if (sceltaShader == 0)  // NONE  v_passthrough
        gl_Position = Projection * View * Model * vec4(aPos, 1.0);
    
    if (sceltaShader == 1 || sceltaShader == 4 ) // GOURAUD_SHADING or NO_TEXTURE
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
    }

    if (sceltaShader == 3 || sceltaShader == 6 ){
        gl_Position = Projection * View * Model * vec4(aPos,1.0);

        vcsPosition = vec3(View * Model * vec4(aPos,1.0));

        vcsLightPosition=vec3(View * vec4(light.position,1.0));

        vcsN = (transpose(inverse(mat3(View * Model)))*aNormal);


    }

    if (sceltaShader == 5) { //wawe
        float a = 1;
        float omega = 1.0;
        vec4 v = vec4(aPos, 1.0);
        float offset = a*sin(omega*time+10.0*aPos.x)*cos(omega*time+10.0*aPos.z);
        v.y=v.y+offset;
        gl_Position = Projection*View*Model*v;
        waveEdge=offset;
    }
    
}  