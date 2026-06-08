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
	// z is up, x from screen
	angles.x += (omega.x * glm::cos(angles.y) + omega.y * glm::sin(angles.y)) * dT;
	angles.y += omega.z * dT; 
	angles.z += (omega.z * glm::cos(angles.x) + omega.x * glm::sin(angles.y) * glm::sin(angles.x) - omega.y * glm::sin(angles.x) * glm::cos(angles.y)) * dT;
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
	rot1 = glm::rotate(rot1, angles.y, glm::vec3(z.x, z.y, z.z));
	x = rot1 * x;
	// theta around new x
	rot2 = glm::rotate(rot2, angles.x, glm::vec3(x.x, x.y, x.z));
	z = rot2 * z;
	// psi around new z
	rot3 = glm::rotate(rot3, angles.z, glm::vec3(z.x, z.y, z.z));
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

void Mesh::setAngularMomentum(glm::vec3 _angularMomentum) {
	angularMomentum = _angularMomentum;
	omega = angularMomentum / MoI;
}
