#version 330 core
layout (location = 0) in vec3 position;
out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform float time;

out vec4 vEyeSpacePos;

void main() {
	vEyeSpacePos = view * vec4(position, 1.0);
    gl_Position = projection * view * vec4(position, 1.0);
    texCoords = position;
}  