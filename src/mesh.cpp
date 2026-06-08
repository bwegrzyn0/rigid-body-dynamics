#include "mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include <iostream>

Mesh::Mesh(float* _vertices, int sizeofVertices, const char* _textureSource, Shader& _shaderProgram, glm::vec3 _pos,  glm::vec3 _color) 
: shaderProgram(_shaderProgram) {
	vertices = _vertices;
	textureSource = _textureSource;
	pos = _pos;
	color = _color;
	if (textureSource != NULL)
		loadTexture();

	// vertices format: 
	// 3f: pos, 2f: texture coords
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeofVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) 0);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) (3*sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Mesh::loadTexture() {
	// TODO	
}

void Mesh::update(float dT) {
	// omega 012 - xyz
	// z is up, x from screen
	angles[0] += (omega[0] * glm::cos(angles[1]) + omega[1] * glm::sin(angles[1])) * dT;
	angles[1] += omega[2] * dT; 
	angles[2] += (omega[2] * glm::cos(angles[0]) + omega[0] * glm::sin(angles[1]) * glm::sin(angles[0]) - omega[1] * glm::sin(angles[0]) * glm::cos(angles[1])) * dT;
	// rotate the model according to euler angles
	model = glm::mat4(1.0f);

	glm::mat4 rot1 = glm::mat4(1.0f);
	glm::mat4 rot2 = glm::mat4(1.0f);
	glm::mat4 rot3 = glm::mat4(1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, pos);
	// z is up, x is from screen
	glm::vec4 x = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 z = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	// phi around z
	rot1 = glm::rotate(rot1, angles[1], glm::vec3(z.x, z.y, z.z));
	x = rot1 * x;
	// theta around new x
	rot2 = glm::rotate(rot2, angles[0], glm::vec3(x.x, x.y, x.z));
	z = rot2 * z;
	// psi around new z
	rot3 = glm::rotate(rot3, angles[2], glm::vec3(z.x, z.y, z.z));
	model = trans * rot3 * rot2 * rot1 * model;
	shaderProgram.setMat4("model", model);
	model = glm::inverse(model);
	model = glm::transpose(model);
	shaderProgram.setMat4("transposeInverseModel", model);
}

void Mesh::render() {
	shaderProgram.setVec3("defColor", color);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
