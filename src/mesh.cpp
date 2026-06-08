#include "mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include <iostream>

Mesh::Mesh(float* _vertices, int sizeofVertices, const char* _textureSource, Shader& _shaderProgram, glm::vec3 _pos,  glm::vec4 _color) 
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*) 0);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*) (3*sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Mesh::loadTexture() {
	// TODO	
}

void Mesh::update() {
	shaderProgram.use();
	model = glm::mat4(1.0f);
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f)); 
	model = glm::translate(model, pos); 
	shaderProgram.setMatrix4fv("model", model);
}

void Mesh::render() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
