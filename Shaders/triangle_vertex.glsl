#version 330 core

layout (location = 0) in vec3 in_coords;
layout (location = 1) in vec3 in_color;
out vec3 out_color;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	out_color = in_color;
	gl_PointSize = 10.0;
	gl_Position = projection * view * model * vec4(in_coords, 1.0);
}