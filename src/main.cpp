#include "main.h"

GLFWwindow* window;

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Set GLFW Major version to OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Set GLFW Minor version to OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Colored triangle", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	/* GLuint programID = LoadShaders("data/shader/SimpleVertexShader.vertexshader", "data/shader/SimpleFragmentShader.fragmentshader"); */
	//GLuint programIDBlue = LoadShaders("data/shader/SimpleVertexShaderBlue.vertexshader", "data/shader/SimpleFragmentShaderBlue.fragmentshader");
    Shader s("data/shader/SimpleVertexShader.vertexshader", "data/shader/SimpleFragmentShader.fragmentshader");
    s.link();
    GLuint programID = s.getProgramId();



	//Create vertex buffer
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	//GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ModelID = glGetUniformLocation(programID, "M");
	GLuint ViewID = glGetUniformLocation(programID, "V");
	GLuint ProjectionID = glGetUniformLocation(programID, "P");
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	glUseProgram(programID);
	glm::vec3 cameraPosition(0,0,0);
	glm::vec3 cameraRotation(0,0,0);
	double mouseY, mouseX;
	int delta = 0;
	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		if (glfwGetKey(window, GLFW_KEY_W))
		{
			cameraPosition.x += MOVE_INCREMENT;
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			cameraPosition.x -= MOVE_INCREMENT;
		}
		if (glfwGetKey(window, GLFW_KEY_A))
		{
			cameraPosition.z -= MOVE_INCREMENT
		}
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			cameraPosition.z += MOVE_INCREMENT;
		}
		if (glfwGetKey(window, GLFW_KEY_Q))
		{
			cameraPosition.z = 0;
			cameraPosition.y = 0;
			cameraPosition.x = 0;
		}
		delta++;
		//Get mouse position
		glfwGetCursorPos(window, &mouseX, &mouseY);
		cameraRotation.y += (mouseX - 1024/2)*0.01f;
		cameraRotation.x += (mouseY - 768/2)*0.01f;

		glfwSetCursorPos(window, 1024/2, 768/2);
	
		//set camera position
		/*glm::mat4 View = glm::lookAt(
			position,
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			);*/
		glm::mat4 View = glm::mat4(
			1.0f
		);
		View = glm::rotate(View, cameraRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		View = glm::rotate(View, cameraRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		View = glm::translate(View, glm::vec3(cameraPosition));

		glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &Projection[0][0]);
		float pos = 3.0f + cos(delta*0.3) + cos(delta*0.1)*3 + cos(delta*0.06)*0.1;
		renderCube(glm::vec3(pos, 0.0f, 0.0f));
		renderCube(glm::vec3(-pos, 0.0f, 0.0f));
		renderCube(glm::vec3(0.0f, pos, 0.0f));
		renderCube(glm::vec3(0.0f, -pos, 0.0f));
		renderCube(glm::vec3(0.0f, 0.0f, pos));
		renderCube(glm::vec3(0.0f, 0.0f, -pos));

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void renderCube(glm::vec3 position)
{
	glm::mat4 Model = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		position.x, position.y, position.z, 1.0f
		);  // Changes for each model !
	glUniformMatrix4fv(2, 1, GL_FALSE, &Model[0][0]);
	//glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 3 indices starting at 0 -> 1 triangle
}

