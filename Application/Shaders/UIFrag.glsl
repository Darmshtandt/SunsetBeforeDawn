#version 420

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec3 VertexTexCoords;
in vec4 VertexColor;
in vec3 FragPosition;

out vec4 Color;

layout(binding = 0) uniform sampler2D Texture;

void main() {
	Color = texture(Texture, VertexTexCoords.xy);
}	