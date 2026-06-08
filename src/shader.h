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
		void setBool(const char* name, bool value) const;
		void setInt(const char* name, int value) const;
		void setFloat(const char* name, float value) const;
		void setMatrix4fv(const char* name, glm::mat4 value) const;
};

