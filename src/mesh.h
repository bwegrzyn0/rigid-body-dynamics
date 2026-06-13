#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

class Mesh {	
	private:
		Shader& shaderProgram;
	public: 
		unsigned int VBO;
		unsigned int VAO;
		glm::vec3 color;
		const char* textureSource;
		unsigned int texture;
		glm::vec3 pos;
		glm::mat4 model; 
		float* vertices;	
		// moments of inertia
		glm::vec3 MoI = glm::vec3(1.0f, 1.0f, 1.0f);
		// principal axes of inertia
		glm::vec3 AoI[3] = {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}; 
		glm::vec3 omega = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 angularMomentum = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 rotationMatrix = glm::mat4(1.0f);

		Mesh(float*  _vertices, int sizeofVertices, const char* _textureSource, Shader& _shaderProgram, glm::vec3 _pos, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f));
		void loadTexture();
		void setAngularMomentum(glm::vec3 _angluarMomentum);
		void update(float dT);
		void render();
};
