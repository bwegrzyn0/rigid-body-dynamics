#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <string>
#include <vector>

// window params
int WIDTH = 1280, HEIGHT = 720;

// cube mesh
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


// adjust window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);	
}

// camera 1. polar angle and 2. asimuth
glm::vec2 cameraAngles = glm::vec2(0.5f, 1.3f);
// velocity of the camera
glm::vec2 cameraV = glm::vec2(0.0f);
// distance of the camera from the origin 
float cameraDistance = 5.0f;
// speed of camera rotation
float cameraVValue = 2.0f;

// input
void processInput(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraV.x = cameraVValue; 
	else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraV.x = -cameraVValue; 
	else 
		cameraV.x = 0.0f;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraV.y = cameraVValue; 
	else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraV.y = -cameraVValue; 
	else
		cameraV.y = 0.0f;
}

unsigned int loadCubemap(std::vector<std::string> faces) {
	unsigned int cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	int txWidth, txHeight, nrChannels;
	unsigned char* data;
	for (int i = 0; i < faces.size(); i++) {
		data = stbi_load(faces[i].c_str(), &txWidth, &txHeight, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, txWidth, txHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		} else {
			std::cout << "Cubemap failed to load: " << faces[i] << std::endl;
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

	return cubemap;
}

int main() {
	// *************************** window *********************************
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
	
	// ******************************************************************
	
	// shader program for meshes
	Shader shader("src/shaders/vertexShader.vs", "src/shaders/fragmentShader.fs");
	shader.use();

	// projection matrix
	glm::mat4 proj = glm::mat4(1.0f);
	float FOV = 45.0f;
	// args: FOV, aspect ratio, near, far
	proj = glm::perspective(FOV, (float(WIDTH))/((float)HEIGHT), 0.1f, 100.0f);
	shader.setMat4("proj", proj);

	// the main cube
	Mesh cube(vertices, sizeof(vertices), NULL, shader, glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f));
	cube.setAngularMomentum(glm::vec3(0.0f, 0.0f, 0.0f));
	// cube.rotationMatrix = glm::rotate(cube.rotationMatrix, 0.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	cube.MoI = glm::vec3(1.0f, 3.0f, 6.0f);

	// light source params (needed by the fragment shader)
	glm::vec3 lightPos = glm::vec3(100.0f, 50.0f, 0.0f);
	shader.setVec3("lightPos", lightPos);

	// cubemap (skybox)
	std::vector<std::string> skybox_faces;
	skybox_faces.push_back("src/sky/sky_right.png");
	skybox_faces.push_back("src/sky/sky_left.png");
	skybox_faces.push_back("src/sky/sky_up.png");
	skybox_faces.push_back("src/sky/sky_down.png");
	skybox_faces.push_back("src/sky/sky_front.png");
	skybox_faces.push_back("src/sky/sky_back.png");
	unsigned int skybox = loadCubemap(skybox_faces);

	Shader skyboxShader("src/shaders/skybox_vertexShader.vs", "src/shaders/skybox_fragmentShader.fs");
	skyboxShader.setMat4("proj", proj);
	// we use the mesh class just to create a VAO and a VBO
	Mesh skyboxMesh(vertices, sizeof(vertices), NULL, shader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));

	// for calculating deltaT
	float lastTime = (float) glfwGetTime();

	while(!glfwWindowShouldClose(window)) {
		processInput(window); 

		float now = (float) glfwGetTime();
		float dT = now - lastTime;
		lastTime = now;

		// view update
		cameraAngles += cameraV * dT;
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -cameraDistance));
		view = glm::rotate(view, cameraAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, cameraAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("view", view);
		skyboxShader.setMat4("view", view);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw the skybox
		glDepthMask(GL_FALSE);
		skyboxShader.use();
		glBindVertexArray(skyboxMesh.VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		// draw other objects
		cube.update(dT);
		cube.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
