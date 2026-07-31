#version 330 core
layout (location = 0) in vec2 aPos; 

out vec2 fragCoord;

void main() {
    fragCoord = aPos * 0.5 + 0.5;  // Converti da -1..1 a 0..1
    gl_Position = vec4(aPos, 0.0, 1.0);
}
