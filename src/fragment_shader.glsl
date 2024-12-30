#version 330 core
in vec4 fragmentColor;
out vec4 pixelColor;

void main() {
    pixelColor = fragmentColor;
}