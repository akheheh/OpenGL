#version 330 core
//texture coordinate input, output from vertex shader
in vec2 textureCoordOutput;
in vec4 fragmentColor;
out vec4 pixelColor;

/*
We want the pixel to read from the texture
If we simply set pixelColor = textureCoordOutput,
then there are issues:
1.) textureCoordOutput is a 2d vector, while pixelColor is a 4d vector
2.) Meaning, either the fragment won't compile for having insufficient data or
3.) The value of pixelColor will only be an RG value, or some other 2-variable color if we use a vec4 with filler values
4.) What we want is to have the pixelColor value equal to a SAMPLE from the texture data the textureCoordOutput refers to
5.) What we need is a way to sample the texture coordinates from the texture and send it as a value for pixelColor
6.) A sampler will do this.  For example, we use sampler2D, a 2D texture sampler
7.) Then, we use the GLSL texture() function which retrieve texture coordinates from a texture
*/

uniform sampler2D aTexture;
void main() {
    pixelColor = texture(aTexture, textureCoordOutput);
}