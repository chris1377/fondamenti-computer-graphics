#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

uniform mat4 Projection;
uniform mat4 Model;

out vec2 fragPos;  // passeremo la posizione al fragment
out vec4 fragColor;

void main() {
    gl_Position = Projection * Model * vec4(position, 0.0, 1.0);
    fragPos = position; // in coordinate locali
    fragColor = color;
}
