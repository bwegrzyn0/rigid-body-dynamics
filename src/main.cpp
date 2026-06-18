#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// window params
int WIDTH = 1280, HEIGHT = 720;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


// zmiana rozmiaru okna
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);	
}

glm::vec3 cameraPos = glm::vec3(0.0f);
glm::vec3 cameraV = glm::vec3(0.0f);
float cameraVValue = 5.0f;

// wejście
void processInput(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraV.z = -cameraVValue; 
	else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraV.z = cameraVValue; 
	else 
		cameraV.z = 0.0f;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraV.x = -cameraVValue; 
	else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraV.x = cameraVValue; 
	else
		cameraV.x = 0.0f;
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraV.y = -cameraVValue; 
	else if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraV.y = cameraVValue; 
	else
		cameraV.y = 0.0f;
}

int main() {
	// okno
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rigid body dynamics", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	
	// shader program for normal cubes
	Shader shaderProgram("src/vertexShader.vert", "src/fragmentShader.frag");
	shaderProgram.use();

	glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(45.0f, (float(WIDTH))/((float)HEIGHT), 0.1f, 100.0f);
	shaderProgram.setMat4("proj", proj);

	Mesh cube(vertices, sizeof(vertices), NULL, shaderProgram, glm::vec3(0.0f, 0.0f, -3.0f),  glm::vec3(1.0f, 0.0f, 0.0f));
	cube.setAngularMomentum(glm::vec3(0.0f, 5.0f, 0.001f));
//	cube.rotationMatrix = glm::rotate(cube.rotationMatrix, 0.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	cube.MoI = glm::vec3(1.0f, 3.0f, 6.0f);

	// shader program for lights
	//Shader lightProgram("src/vertexShader.vert", "src/lightFragmentShader.frag");
	//lightProgram.setMat4("proj", proj);

	glm::vec3 lightPos = glm::vec3(2.0f, 2.0f, 4.0f);
	//Mesh light(vertices, sizeof(vertices), NULL, lightProgram, lightPos, glm::vec3(1.0f));
	shaderProgram.setVec3("lightPos", lightPos);

	float lastTime = (float) glfwGetTime();

	while(!glfwWindowShouldClose(window)) {
		processInput(window); 
		float now = (float) glfwGetTime();
		float dT = now - lastTime;
		lastTime = now;

		cameraPos += cameraV * dT;
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, -cameraPos);
		shaderProgram.setMat4("view", view);
		//lightProgram.setMat4("view", view);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.update(dT);
		cube.render();

		//light.update(dT);
		//light.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
