#version 400
in vec3 colour;
in vec2 passTexture;
out vec4 fragmentColour;
uniform sampler2D diffuseTexture;

uniform float alpha;


void main() {
 vec4 textureColor = texture(diffuseTexture, passTexture);
 textureColor.a = alpha;
 fragmentColour = textureColor;
}