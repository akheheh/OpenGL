#version 330 core
in vec2 texCoord;
in vec4 fragmentColor;
out vec4 pixelColor;

uniform sampler2D aTexture;
void main() {
    pixelColor = texture(aTexture, texCoord);
}