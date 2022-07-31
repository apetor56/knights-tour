#version 330

layout(location = 0) in vec2 iPosition;
layout(location = 1) in float iColor;

out vec4 oColor;

void main() {
    gl_Position = vec4(iPosition.x, iPosition.y, 0.0, 1.0);
    oColor = vec4(iColor * 1, iColor * 1, iColor * 1, 1.0);
}