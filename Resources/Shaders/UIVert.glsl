#version 420 compatibility	

uniform mat4 Projection;
uniform mat4 World;

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec3 Texcoord;
layout(location = 3) in vec4 Color;

out mat4 PWV;
out vec3 VertexPosition;
out vec3 VertexNormal;
out vec3 VertexTexCoords;
out vec4 VertexColor;
out vec3 FragPosition;

void main() {
	mat4 model = World;
	PWV = Projection * model;

	gl_Position = PWV * Position;
							
	VertexPosition = vec3(Position);
	VertexNormal = vec3(Normal);
	VertexTexCoords = Texcoord;
	VertexColor = Color;
	FragPosition = vec3(World * Position);
}