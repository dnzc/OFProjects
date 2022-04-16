#version 150

uniform mat4 modelViewProjectionMatrix;

in vec2 texcoord;

uniform sampler2DRect agentTex;

void main() {
    // read position data from texture
    gl_Position = modelViewProjectionMatrix * texture(agentTex, texcoord);
}
