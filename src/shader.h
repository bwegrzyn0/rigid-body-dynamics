#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
	public:
		unsigned int shaderProgram;

		Shader(const char* vertexPath, const char* fragmentPath);
		void use();
		void setBool(const char* name, bool value) ;
		void setInt(const char* name, int value) ;
		void setFloat(const char* name, float value) ;
		void setMat4(const char* name, glm::mat4 value) ;
		void setVec4(const char* name, glm::vec4 value) ;
		void setVec3(const char* name, glm::vec3 value) ;
};

