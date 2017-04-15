#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;
uniform float time;
uniform bool sepia = false;

float foo;

void main() {   
    color = texture(skybox, TexCoords);
    if (sepia) {
	    foo = max(max(color.x, color.y), color.z);
	    color = vec4(foo * 0.85, foo * 0.8, 0.55 * foo, 1.0);
	};
}