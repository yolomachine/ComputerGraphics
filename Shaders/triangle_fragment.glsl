#version 330 core

in vec3 out_color;
out vec4 resulting_color;

uniform float time;
uniform bool sepia = false;

float foo;

void main() {
	resulting_color = vec4(out_color.x + sin(time), out_color.y + cos(time), out_color.z, cos(time) + 1.1);
	if (sepia) {
		foo = max(max(resulting_color.x, resulting_color.y), resulting_color.z);
	    resulting_color = vec4(foo * 0.85, foo * 0.8, 0.55 * foo, 1.0);
	};
}