#include "mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "shader.h"
#include <iostream>

Mesh::Mesh(float* _vertices, int sizeofVertices, const char* _textureSource, Shader& _shader, glm::vec3 _pos,  glm::vec3 _color) : shader(_shader) {
	vertices = _vertices;
	textureSource = _textureSource;
	pos = _pos;
	color = _color;
	if (textureSource != NULL)
		loadTexture();

	// vertices format: 
	// 3f: pos, 3f: normals 
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
}

void Mesh::update(float dT) {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, pos);
	model = glm::mat4(1.0f);

	glm::vec3* AoI_worldspace = new glm::vec3[3];
	AoI_worldspace[0] = glm::vec3(rotationMatrix * glm::vec4(AoI[0], 1.0f));
	AoI_worldspace[1] = glm::vec3(rotationMatrix * glm::vec4(AoI[1], 1.0f));
	AoI_worldspace[2] = glm::vec3(rotationMatrix * glm::vec4(AoI[2], 1.0f));

	omega.x += (MoI[1] - MoI[2]) / MoI[0] * omega.y * omega.z * dT;
	float omega_2 = (MoI[2] - MoI[0]) / MoI[1] * omega.x;
	float omega_3 = -(MoI[0] - MoI[1]) / MoI[2] * omega.x;
	float sqrt_omegas = glm::sqrt(omega_2 * omega_3);
	float k = glm::sqrt(-((MoI[0] - MoI[1]) * MoI[1]) / (MoI[2] * (MoI[2] - MoI[0])));
	// omega.y dot = omega_2 * omega.z
	// omega.z dot = -omega_3 * omega.y
	// system of coupled diff eq
	float phase = glm::atan(omega.z / k, omega.y);	
	// std::cout << phase << std::endl;
	if (phase != phase)
		phase = 0;
	float A = glm::sqrt(omega.y * omega.y + omega.z * omega.z / (k * k));
	if (omega.y == 0 && omega.z >= 0) {
		omega.y = -A * glm::sin(sqrt_omegas * dT);
		omega.z = A * k * glm::cos(sqrt_omegas * dT);
	} else if (omega.y == 0 && omega.z < 0) { // correction when atan returns pi (numerical error accumulates leading to inaccurate results)
		omega.y = A * glm::sin(sqrt_omegas * dT);
		omega.z = -A * k * glm::cos(sqrt_omegas * dT);
	} else {
		omega.y = A * glm::cos(sqrt_omegas * dT + phase);
		omega.z = A * k * glm::sin(sqrt_omegas * dT + phase);
	}

	// nearby values are conserved, those oscillate a bit
	float E = omega.x * omega.x * MoI[0] + omega.y * omega.y * MoI[1] + omega.z * omega.z * MoI[2];
	float M = omega.x * omega.x * MoI[0] * MoI[0] + omega.y * omega.y * MoI[1] * MoI[1] + omega.z * omega.z * MoI[2] * MoI[2];
	//std::cout << "Energy: " <<  E << std::endl;
	//std::cout << "Angular momentum: " << M << std::endl;

	// rotate the model appropriately
	float omegaLength = glm::length(omega);
	glm::vec3 omega_worldspace = glm::vec3(glm::inverse(rotationMatrix) * glm::vec4(omega, 1.0f));
	if (omegaLength != 0)
		rotationMatrix = glm::rotate(rotationMatrix, omegaLength * dT, omega_worldspace);
	// apply all the transformations
	model = trans * rotationMatrix * model;
	shader.setMat4("model", model);
	// transpose inverse for the normals (it's costly for the shader so we do it here)
	model = glm::inverse(model);
	model = glm::transpose(model);
	shader.setMat4("transposeInverseModel", model);
}

void Mesh::render() {
	shader.setVec3("defColor", color);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Mesh::setAngularMomentum(glm::vec3 _angularMomentum) {
	angularMomentum = _angularMomentum;
	// compute omega based on the angular momentum and orientation
	glm::vec3* AoI_worldspace = new glm::vec3[3];
	AoI_worldspace[0] = glm::vec3(rotationMatrix * glm::vec4(AoI[0], 1.0f));
	AoI_worldspace[1] = glm::vec3(rotationMatrix * glm::vec4(AoI[1], 1.0f));
	AoI_worldspace[2] = glm::vec3(rotationMatrix * glm::vec4(AoI[2], 1.0f));
	omega.x = glm::dot(angularMomentum, AoI_worldspace[0]) / MoI[0];
	omega.y = glm::dot(angularMomentum, AoI_worldspace[1]) / MoI[1];
	omega.z = glm::dot(angularMomentum, AoI_worldspace[2]) / MoI[2];
}
