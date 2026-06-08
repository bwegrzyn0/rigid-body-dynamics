#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
	// pos = P * V * M * vec4(x,y,z,1)
	// P - projection matrix
	// V - view matrix
	// M - model matrix
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	// gl_Position = vec4(aPos, 1.0f);
	texCoord = aTexCoord;
}
	
