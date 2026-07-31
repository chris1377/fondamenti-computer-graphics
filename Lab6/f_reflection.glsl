#version 450 core

in vec3 Normal;
in vec3 Position;
out vec4 FragColor;

uniform vec3 ViewPos;
uniform samplerCube cubemap;

void main(){

	vec3 E = normalize(Position - ViewPos);
	vec3 R = -reflect(E,normalize(Normal));
	FragColor = vec4(texture(cubemap,R).rgb,1.0);

}