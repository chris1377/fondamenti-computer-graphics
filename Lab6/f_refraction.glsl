#version 450 core

in vec3 Normal;
in vec3 Position;
out vec4 FragColor;

uniform vec3 ViewPos;
uniform samplerCube cubemap;

void main(){

	vec3 E = normalize(Position - ViewPos);
	float ratio = 1.00/1.52;
	vec3 R = refract(E, normalize(Normal),ratio);
	FragColor = vec4(texture(cubemap,R).rgb,1.0);

}