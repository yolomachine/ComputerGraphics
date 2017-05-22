#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoords;
layout (location = 3) in vec3 normal; 

out vec3 newColor;
out vec3 fragPos;
out vec3 newNormal;
out vec2 newTextureCoords;
out vec4 vEyeSpacePos;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	newColor = color;
    fragPos = vec3(model * vec4(position, 1.0f));
    newNormal = mat3(transpose(inverse(model))) * normal;  
	newTextureCoords = textureCoords;

	vEyeSpacePos = view * model * vec4(position, 1.0);
	gl_Position = projection * view * model * vec4(position, 1.0);
}