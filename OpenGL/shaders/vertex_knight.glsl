#version 430

layout(location = 0) in vec2 iPosition;
layout(location = 2) in vec2 iTexcoord;

out vec2 Texcoord;

uniform mat4 transformMatrix;

void main() {
    gl_Position = transformMatrix * vec4(iPosition.x, iPosition.y, 0.0, 1.0);
    Texcoord = iTexcoord;
}