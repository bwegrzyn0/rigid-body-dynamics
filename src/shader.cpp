#include <glad/glad.h>
#include <string>
#include <iostream>
#include "shader.h"
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// load sources for both shaders
	std::ifstream vertex(vertexPath);
	std::ifstream fragment(fragmentPath);

	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	if (!vertex.is_open()) {
		std::cout << "Error opening vertex shader" << std::endl;
	}

	if (!fragment.is_open()) {
		std::cout << "Error opening fragment shader" << std::endl;
	}

	std::stringstream vertexSourceStream, fragmentSourceStream;
	vertexSourceStream << vertex.rdbuf();
	fragmentSourceStream << fragment.rdbuf();
	vertex.close();
	fragment.close();
	vertexShaderSource = vertexSourceStream.str();
	fragmentShaderSource = fragmentSourceStream.str();	

	const char* vertexShaderCode = vertexShaderSource.c_str();
	const char* fragmentShaderCode = fragmentShaderSource.c_str();

	// compile shaders
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
	}

	// create shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Shader program linking failed: " << infoLog << std::endl;
	}

	// cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// use this program
void Shader::use() {
	glUseProgram(shaderProgram);
}

// setters for different types
void Shader::setBool(const char* name, bool value) const {
	glUniform1i(glGetUniformLocation(shaderProgram, name), (int)value);
}

void Shader::setInt(const char* name, int value) const {
	glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::setFloat(const char* name, float value) const {
	glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::setMatrix4fv(const char* name, glm::mat4 value) const {
	// args: location, number of matricies, transpose, matrix4fv
	// glm::value_ptr(value), because glm type is incompatible with the opengl one
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, glm::value_ptr(value));
}
