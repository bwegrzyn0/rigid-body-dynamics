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
		float MoI[3] = {1.0f, 1.0f, 1.0f};
		// principal axes of inertia
		glm::vec3 AoI[3] = {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}; 
		// euler angles; theta, phi, psi
		float angles[3] = {0.0f, 0.0f, 0.0f};
		float omega[3] = {0.0f, 0.0f, 0.0f};

		Mesh(float*  _vertices, int sizeofVertices, const char* _textureSource, Shader& _shaderProgram, glm::vec3 _pos, glm::vec3 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		void loadTexture();
		void update(float dT);
		void render();
};
