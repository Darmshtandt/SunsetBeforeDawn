#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 Projection;
uniform mat4 World;
uniform mat4 View;

uniform int IsTerrain = 0;
uniform float time;
uniform float grassHeight = 0.5; // Высота травы
uniform float windStrength = 0.1; // Сила ветра

in mat4 PWV[];
in vec3 vVertexPosition[];
in vec3 vVertexNormal[];
in vec3 vVertexTexCoords[];
in vec4 vVertexColor[];
in vec3 vFragPosition[];

out vec3 VertexPosition;
out vec3 VertexNormal;
out vec3 VertexTexCoords;
out vec4 VertexColor;
out vec3 FragPosition;

void main() {
    if (IsTerrain) {
        for (int i = 0; i < gl_in.length(); ++i) {
            gl_Position = gl_in[i].gl_Position;
            VertexPosition = vVertexPosition[i];
            VertexNormal = vVertexNormal[i];
            VertexTexCoords = vVertexTexCoords[i];
            VertexColor = vVertexColor[i];
            FragPosition = vFragPosition[i];
            EmitVertex();
        }
        EndPrimitive();
    }

    vec3 basePos = vec3(0, 0, 0);

    // Колыхание травы — простой синус для ветра
    float windOffset = sin(time + basePos.x * 2.0) * windStrength;

    // Вершины квадрата (двойной треугольник) для травинки
    vec3 p1 = basePos + 16384 * vec3(-0.1, 0.0, 0.0); // Левый низ
    vec3 p2 = basePos + 16384 * vec3(0.1, 0.0, 0.0);  // Правый низ
    vec3 p3 = basePos + 16384 * vec3(-0.1 + windOffset, grassHeight, 0.0); // Левый верх
    vec3 p4 = basePos + 16384 * zvec3(0.1 + windOffset, grassHeight, 0.0);  // Правый верх

    // Проекция в экранные координаты
    gl_Position = PWV[0] * vec4(p1, 1.0);
    VertexPosition = p1;
    VertexNormal = vec3(0, 1, 0);
    VertexTexCoords = vec3(0.0, 0.0, 0.0);
    VertexColor = vec4(1, 1, 1, 1);
    FragPosition = gl_Position.xyz;
    EmitVertex();

    gl_Position = PWV[0] * vec4(p2, 1.0);
    VertexPosition = p2;
    VertexNormal = vec3(0, 1, 0);
    VertexTexCoords = vec3(0.0, 0.0, 0.0);
    VertexColor = vec4(1, 1, 1, 1);
    FragPosition = gl_Position.xyz;
    EmitVertex();

    gl_Position = PWV[0] * vec4(p3, 1.0);
    VertexPosition = p3;
    VertexNormal = vec3(0, 1, 0);
    VertexTexCoords = vec3(0.0, 0.0, 0.0);
    VertexColor = vec4(1, 1, 1, 1);
    FragPosition = gl_Position.xyz;
    EmitVertex();

    EndPrimitive();
}