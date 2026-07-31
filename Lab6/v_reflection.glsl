#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

void main(){

	Normal = mat3(transpose(inverse(Model)))*aNormal;
	Position = vec3(Model * vec4(aPos,1.0));
	gl_Position = Projection * View * Model * vec4(aPos,1.0);

}