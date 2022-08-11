#version 430

in vec2 Texcoord;
out vec4 fragColor;

uniform sampler2D knightTex;

void main() {
    vec4 texColor = texture(knightTex, Texcoord);
    if(texColor.a < 0.2) {
        discard;
    }

    fragColor = texColor;
}