#version 330 core

in vec2 fragPos;
in vec4 fragColor;

out vec4 outColor;

void main() {
    // Ombra
    float shadowFactor = 1.0 - clamp((fragPos.y + 1.0) / 3.0, 0.0, 1.0);
    vec3 shadowColor = fragColor.rgb * (0.7 + 0.3 * shadowFactor); 
    outColor = vec4(shadowColor, fragColor.a);
}
