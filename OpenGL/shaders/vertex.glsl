#version 430

layout(location = 0) in vec2 iPosition;
layout(location = 1) in float iColor;

out vec4 oColor;

void main() {
    gl_Position = vec4(iPosition.x, iPosition.y, 0.0, 1.0);
    if(iColor == 1.0) {
        oColor = vec4(0.7, 0.8, 0.7, 1.0);
    }
    else {
        oColor = vec4(0.35, 0.5, 0.3, 1.0);
    }
}