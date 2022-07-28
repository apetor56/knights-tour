#version 330

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iColor;

out vec4 oColor;

void main() {
    gl_Position = vec4(iPosition.x, iPosition.y, iPosition.z, 1.0);
    oColor = vec4(iColor.x, iColor.y, iColor.z, 1.0);
}