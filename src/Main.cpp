#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stb_image.h>
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include"Camera.h"
#include<vector>
//Called when window resizes to change the viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//Function to process all input
void processInput(GLFWwindow* window);

GLfloat vertices[] = {
	//     COORDINATES      TexCoord   /       NORMALS       //
	-0.5f, -0.5f, -0.5f,      0.0f, 0.0f,    0.0f,  0.0f, -1.0f, // Back face
	 0.5f, -0.5f, -0.5f,      1.0f, 0.0f,    0.0f,  0.0f, -1.0f, // Back face
	 0.5f,  0.5f, -0.5f,      1.0f, 1.0f,    0.0f,  0.0f, -1.0f, // Back face
	-0.5f,  0.5f, -0.5f,      0.0f, 1.0f,    0.0f,  0.0f, -1.0f, // Back face

	-0.5f, -0.5f,  0.5f,      0.0f, 0.0f,    0.0f,  0.0f,  1.0f, // Front face
	 0.5f, -0.5f,  0.5f,      1.0f, 0.0f,    0.0f,  0.0f,  1.0f, // Front face
	 0.5f,  0.5f,  0.5f,      1.0f, 1.0f,    0.0f,  0.0f,  1.0f, // Front face
	-0.5f,  0.5f,  0.5f,      0.0f, 1.0f,    0.0f,  0.0f,  1.0f, // Front face

	-0.5f,  0.5f,  0.5f,      0.0f, 0.0f,   -1.0f,  0.0f,  0.0f, // Left face
	-0.5f,  0.5f, -0.5f,      1.0f, 0.0f,   -1.0f,  0.0f,  0.0f, // Left face
	-0.5f, -0.5f, -0.5f,      1.0f, 1.0f,   -1.0f,  0.0f,  0.0f, // Left face
	-0.5f, -0.5f,  0.5f,      0.0f, 1.0f,   -1.0f,  0.0f,  0.0f, // Left face

	 0.5f,  0.5f,  0.5f,      0.0f, 0.0f,    1.0f,  0.0f,  0.0f, // Right face
	 0.5f,  0.5f, -0.5f,      1.0f, 0.0f,    1.0f,  0.0f,  0.0f, // Right face
	 0.5f, -0.5f, -0.5f,      1.0f, 1.0f,    1.0f,  0.0f,  0.0f, // Right face
	 0.5f, -0.5f,  0.5f,      0.0f, 1.0f,    1.0f,  0.0f,  0.0f, // Right face

	-0.5f, -0.5f, -0.5f,      0.0f, 0.0f,    0.0f, -1.0f,  0.0f, // Bottom face
	 0.5f, -0.5f, -0.5f,      1.0f, 0.0f,    0.0f, -1.0f,  0.0f, // Bottom face
	 0.5f, -0.5f,  0.5f,      1.0f, 1.0f,    0.0f, -1.0f,  0.0f, // Bottom face
	-0.5f, -0.5f,  0.5f,      0.0f, 1.0f,    0.0f, -1.0f,  0.0f, // Bottom face

	-0.5f,  0.5f, -0.5f,      0.0f, 0.0f,    0.0f,  1.0f,  0.0f, // Top face
	 0.5f,  0.5f, -0.5f,      1.0f, 0.0f,    0.0f,  1.0f,  0.0f, // Top face
	 0.5f,  0.5f,  0.5f,      1.0f, 1.0f,    0.0f,  1.0f,  0.0f, // Top face
	-0.5f,  0.5f,  0.5f,      0.0f, 1.0f,    0.0f,  1.0f,  0.0f  // Top face
};


GLuint indices[] = {
	0,  2,  1,   0,  3,  2,  // Back face (CCW)
	4,  5,  6,   4,  6,  7,  // Front face (CCW)

	8,  9, 10,   8, 10, 11,  // Left face (CCW)
	12, 14, 13,  12, 15, 14, // Right face (CCW)

	16, 17, 18,  16, 18, 19, // Bottom face (CCW)
	20, 22, 21,  20, 23, 22  // Top face (CCW)
};
//Cube Positions
std::vector<glm::vec3> cubePositions;
//Light Source Vertices
GLfloat lightsource[] = {
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f

};
//Indices for light source
GLuint lightsourceIndices[] = {
	0, 2, 1,
	0, 3, 2,
	0, 7, 4,
	0, 3, 7,
	3, 6, 7,
	3, 2, 6,
	2, 5, 6,
	2, 1, 5,
	1, 4, 5,
	1, 0, 4,
	4, 6, 5,
	4, 7, 6
};

glm::vec3 pointLightPositions[4] = {
	glm::vec3(0.0f),
	glm::vec3(0.0f),
	glm::vec3(0.0f),
	glm::vec3(0.0f)
};
	
int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "LEARNIN", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);


	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));
	// Links VBO to VAO
	VAO1.LinkAttbr(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttbr(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttbr(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	// Create the light source vao
	VAO lightVAO;
	lightVAO.Bind();
	VBO lightVBO(lightsource, sizeof(lightsource));
	EBO lightEBO(lightsourceIndices, sizeof(lightsourceIndices));
	lightVAO.LinkAttbr(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	//Shader Init
	Shader shaderProgram("D:/Learnin OPENGL/Understood Well Till Here/src/shader/vertex.glsl", "D:/Learnin OPENGL/Understood Well Till Here/src/shader/fragment.glsl");
	Shader lightShader("D:/Learnin OPENGL/Understood Well Till Here/src/shader/lightVertex.glsl", "D:/Learnin OPENGL/Understood Well Till Here/src/shader/lightFragment.glsl");
	
	//SpotLight Values 
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	//Creating Box transformation matrices
	glm::vec3 BoxPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 BoxModel = glm::mat4(1.0f);
	//Rotate Box

	//Load Texture
	Texture diffuse("D:/Learnin OPENGL/Understood Well Till Here/resources/container2.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture specular("D:/Learnin OPENGL/Understood Well Till Here/resources/container2_specular.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	//Set Material Properties for the Box to be rendered
	shaderProgram.Activate();
	shaderProgram.setInt("material.specular", 0);
	shaderProgram.setInt("material.diffuse", 1);
	shaderProgram.setFloat("material.shininess", 300.0f);
	//Set SpotLight Properties
	shaderProgram.setFloat("light.constant", 1.0f);
	shaderProgram.setFloat("light.linear", 0.09f);
	shaderProgram.setFloat("light.quadratic", 0.032f);
	shaderProgram.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	shaderProgram.setVec3("light.diffuse", 1.7f, 1.7f, 1.7f); // darken diffuse light a bit
	shaderProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	shaderProgram.setFloat("light.cutOff", glm::cos(glm::radians(4.5f)));
	shaderProgram.setFloat("light.outerCutOff", glm::cos(glm::radians(15.5f)));
	//Set Point Light Properties
	for (unsigned int i = 0; i < 4; i++) {
		shaderProgram.setFloat("pointLight[" + std::to_string(i) + "].constant", 1.0f);
		shaderProgram.setFloat("pointLight[" + std::to_string(i) + "].linear", 0.09f);
		shaderProgram.setFloat("pointLight[" + std::to_string(i) + "].quadratic", 0.032f);
		shaderProgram.setVec3("pointLight[" + std::to_string(i) + "].ambient", 0.6f, 0.6f, 0.6f);
		shaderProgram.setVec3("pointLight[" + std::to_string(i) + "].diffuse", 1.0f, 1.0f, 1.0f);
		shaderProgram.setVec3("pointLight[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
	}
	//Set Directional Light Properties
	shaderProgram.setVec3("dirLight.direction", 0.0f, -6.0f, 0.0f);
	shaderProgram.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
	shaderProgram.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shaderProgram.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	
			for (unsigned int k = 0; k < 200; k++) 
				cubePositions.push_back(glm::vec3(  1.1f,   -1.1f,  k* 1.1f));
			
		
		
	
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // Cull back faces
	glFrontFace(GL_CW); // Set front faces to counterclockwise winding
	//Init Camera Object
	Camera camera(800, 800, glm::vec3(0.0f, 0.0f, 3.0f));
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		//Calculate Delta Time
		GLfloat currentFrame = glfwGetTime();
		camera.deltaTime = currentFrame - camera.lastTime;
		camera.lastTime = currentFrame;
		//Display FPS
		std::cout << 1 / camera.deltaTime << std::endl;
		processInput(window);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		specular.Bind();
		diffuse.Bind();

		//Rotate Light Source
		lightPos = camera.Position;
		//Changing Light Source Color
		//Camera stuff
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "camMatrix");
		camera.Inputs(window);
		shaderProgram.setVec3("light.Position", lightPos.x, lightPos.y, lightPos.z);
		shaderProgram.setVec4("light.Color", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		shaderProgram.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		shaderProgram.setVec3("light.Direction", camera.Orientation.x, camera.Orientation.y, camera.Orientation.z);
		//For loop to change light Position 
		for (unsigned int i = 0; i < 4; i++) {

			pointLightPositions[i] = glm::vec3( 8+(sin(2*glfwGetTime()+i ) * 15.0f), i*-12.0f, 8 + (cos(2*glfwGetTime()+i ) * 15.0f));
			shaderProgram.setVec3("pointLight[" + std::to_string(i) + "].Position", pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			shaderProgram.setVec4("pointLight[" + std::to_string(i) + "].Color", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		}
		//Bind Texture
	
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		for (unsigned int i = 0; i < cubePositions.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			shaderProgram.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		//Light Shader
		lightShader.Activate();
		lightShader.setVec4("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		for (unsigned int i = 0; i < 4; i++) {

			lightModel = glm::mat4(1.0f);
			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(2.0f)); // Make it a smaller cube
			lightShader.setMat4("model", lightModel);
			glDrawElements(GL_TRIANGLES, sizeof(lightsourceIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	diffuse.Delete();
	specular.Delete();
	shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();


	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}