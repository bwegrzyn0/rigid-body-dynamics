#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 transposeInverseModel;

void main() {
	// pos = P * V * M * vec4(x,y,z,1)
	// P - projection matrix
	// V - view matrix
	// M - model matrix
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	fragPos = vec3(model * vec4(aPos, 1.0f));
	normal = mat3(transposeInverseModel) * aNormal;
}
	
