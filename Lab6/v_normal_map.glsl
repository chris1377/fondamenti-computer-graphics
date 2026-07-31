// Vertex shader: Normal mapping shading
// ================
#version 450 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;         // position variable has attribute position 0
layout (location = 1) in vec4 aColor;       // color variable has attribute position 1
layout (location = 2) in vec3 aNormal;      // normal variable has attribute position 2
layout (location = 3) in vec2 coord_st;     // texture variable has attribute position 3
layout (location = 4) in vec3 Tangent;      // tangent variable has attribute position 4

out vec2 _TexCoord;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

// Values that stay constant for the whole mesh.
uniform mat4 Projection;  
uniform mat4 Model;
uniform mat4 View;
uniform vec3 ViewPos;

struct PointLight{
	vec3 position;
	vec3 color;
	float power;
 };
uniform PointLight light;

void main()
{
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	// we pass the texture coordinate to the fragment shader
	_TexCoord = coord_st;
	
    mat3 normalMatrix = transpose(inverse(mat3(Model)));
    vec3 T = normalize(normalMatrix * Tangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);

    vec3 B = normalize(cross(N, T));

    mat3 TBNinv = transpose(mat3(T, B, N));    

	// We use the TBNinv matrix to transform world space vectors in tangent space
	TangentLightPos = TBNinv * light.position;
    TangentViewPos  = TBNinv * ViewPos;
    TangentFragPos  = TBNinv * vec3( Model * vec4(aPos, 1.0));
} 