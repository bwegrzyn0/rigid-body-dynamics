#include "mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include <iostream>

Mesh::Mesh(float* _vertices, int sizeofVertices, const char* _textureSource, Shader& _shaderProgram, glm::vec3 _pos,  glm::vec3 _color) : shaderProgram(_shaderProgram) {
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

glm::vec3 eps = glm::vec3(1.0f);

void Mesh::update(float dT) {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, pos);
	model = glm::mat4(1.0f);

	// compute omega based on the angular momentum and orientation
	glm::vec3* AoI_worldspace = new glm::vec3[3];
	AoI_worldspace[0] = glm::vec3(rotationMatrix * glm::vec4(AoI[0], 1.0f));
	AoI_worldspace[1] = glm::vec3(rotationMatrix * glm::vec4(AoI[1], 1.0f));
	AoI_worldspace[2] = glm::vec3(rotationMatrix * glm::vec4(AoI[2], 1.0f));
	omega.x = glm::dot(angularMomentum, AoI_worldspace[0]) / MoI[0];
	omega.y = glm::dot(angularMomentum, AoI_worldspace[1]) / MoI[1];
	omega.z = glm::dot(angularMomentum, AoI_worldspace[2]) / MoI[2];

	float omegaLength = glm::length(omega);
	glm::vec3 omega_worldspace = glm::vec3(glm::inverse(rotationMatrix) * glm::vec4(omega, 1));

	rotationMatrix = glm::rotate(rotationMatrix, omegaLength * dT, omega_worldspace);	

	// std::cout << omegaLength << std::endl;

	model = trans * rotationMatrix * model;

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
