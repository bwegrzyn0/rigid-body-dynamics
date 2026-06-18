#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 proj;

void main() {
	// pos = P * V * vec4(x,y,z,1)
	// P - projection matrix
	// V - view matrix
	vec4 pos = (proj * mat4(mat3(view)) * vec4(aPos, 1.0f));
	gl_Position = pos.xyww;
	TexCoords = aPos;
}
	
