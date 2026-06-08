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
		glm::vec4 color;
		const char* textureSource;
		unsigned int texture;
		glm::vec3 pos;
		glm::mat4 model; 
		float* vertices;	

		Mesh(float*  _vertices, int sizeofVertices, const char* _textureSource, Shader& _shaderProgram, glm::vec3 _pos, glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		void loadTexture();
		void update();
		void render();
};
