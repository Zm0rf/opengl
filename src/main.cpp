#include "main.h"

GLFWwindow* window;
GameContext context;

int main(void)
{
    if( !initContext(&context) )
    {
        fprintf(stderr, "Failed to initialize application\n");
        return -1;
    }

    window = context.window;

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    /* GLuint programID = LoadShaders("data/shader/SimpleVertexShader.vertexshader", "data/shader/SimpleFragmentShader.fragmentshader"); */
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
	glUseProgram(programID);

    WorldChunk chunk;

	glm::vec3 cameraPosition(0,0,0);
	glm::vec3 cameraRotation(0,0,0);
    glm::vec3 velocity(0,0,0);
	double mouseY, mouseX;

    do
    {
        context.time_last_frame = context.time_now;
        context.time_now = glfwGetTime();
        context.time_delta = context.time_now - context.time_last_frame;

        glm::mat4 Projection = glm::perspective(
                45.0f,
                /* 4.0f / 3.0f, */
                (float)context.width / (float)context.height,
                0.1f,
                100.0f);

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

        // physics :D (what causes the velocity to be inverted?)
        velocity.y += 0.1f;
        if( cameraPosition.y >= 0 )
        {
            cameraPosition.y = 0;
            velocity.y = 0;
        }

        // Actor rotation
        glfwGetCursorPos(window, &mouseX, &mouseY);
        glfwSetCursorPos(window, context.width/2, context.height/2);
        cameraRotation.y += (mouseX - context.width/2)*0.01f;
        cameraRotation.x += (mouseY - context.height/2)*0.01f;
        // Clamp the rotation not to loop
        if( cameraRotation.x < -0.5f*PI )
        {
            cameraRotation.x = -0.5f*PI;
        }
        else if( cameraRotation.x > 0.5f*PI )
        {
            cameraRotation.x = 0.5f*PI;
        }

        // Actor movement
        glm::vec3 dist;
        if (glfwGetKey(window, GLFW_KEY_W))
        {
            dist.z += MOVE_INCREMENT;
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
            dist.z -= MOVE_INCREMENT;
        }
        if (glfwGetKey(window, GLFW_KEY_A))
        {
            dist.x += MOVE_INCREMENT
        }
        if (glfwGetKey(window, GLFW_KEY_D))
        {
            dist.x -= MOVE_INCREMENT;
        }
        if( glfwGetKey(window, GLFW_KEY_SPACE) )
        {
            velocity.y = -1.0f;
        }
        dist = glm::rotate(dist, cameraRotation.y, glm::vec3(0.0f, -1.0f, 0.0f));
        cameraPosition += dist;
        cameraPosition += velocity;
        if (glfwGetKey(window, GLFW_KEY_Q))
        {
            cameraPosition = glm::vec3(0.0f);
            cameraRotation = glm::vec3(0.0f);
        }
	
		//set camera position
		/*glm::mat4 View = glm::lookAt(
			position,
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			);*/
		glm::mat4 View = glm::mat4(1.0f);
		View = glm::rotate(View, cameraRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		View = glm::rotate(View, cameraRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		View = glm::translate(View, cameraPosition);

		glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &Projection[0][0]);

        // Make the cubes move :D
        float pos = 3.0f;
        pos += cos(context.time_now*2.0f);
        pos += cos(context.time_now*5.0f)*0.3;
        if( glfwGetKey(context.window, GLFW_KEY_E) == GLFW_PRESS )
        {
            pos += cos(context.time_now*100.0f)*0.04;
        }

		renderCube(glm::vec3(pos, 0.0f, 0.0f));
		renderCube(glm::vec3(-pos, 0.0f, 0.0f));
		renderCube(glm::vec3(0.0f, pos, 0.0f));
		renderCube(glm::vec3(0.0f, -pos, 0.0f));
		renderCube(glm::vec3(0.0f, 0.0f, pos));
		renderCube(glm::vec3(0.0f, 0.0f, -pos));

        chunk.render();

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (
            (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) &&
            (glfwWindowShouldClose(window) == 0) );

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

bool initContext(GameContext* context)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Set GLFW Major version to OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Set GLFW Minor version to OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    context->width = 1024;
    context->height = 768;

	context->window = glfwCreateWindow(
            context->width, context->height,
            "OPENGL TEST :D",
            NULL, NULL);

    glfwSetWindowSizeCallback(context->window, onWindowResize);

	if (context->window == NULL)
    {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(context->window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
    {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(context->window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glViewport(0, 0, context->width, context->height);
    return true;
}
void onWindowResize(GLFWwindow* window, int width, int height)
{
    context.width = width;
    context.height = height;
    glViewport(0, 0, context.width, context.height);
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

WorldChunk::WorldChunk():
    chunk_x(0), chunk_y(0), chunk_z(0)
{
    for( int x=0; x<CHUNK_SIZE; x++ )
    {
        for( int y=0; y<CHUNK_SIZE; y++ )
        {
            for( int z=0; z<CHUNK_SIZE; z++ )
            {
                this->data[x][y][z] = false;
                if( x == y && y == z )
                    this->data[x][y][z] = true;
            }
        }
    }
    chunk_x += 3;
    chunk_z += 3;
}
WorldChunk::~WorldChunk()
{
}
void WorldChunk::render()
{
    for( int x=0; x<CHUNK_SIZE; x++ )
    {
        for( int y=0; y<CHUNK_SIZE; y++ )
        {
            for( int z=0; z<CHUNK_SIZE; z++ )
            {
                if( this->data[x][y][z] )
                {
                    renderCube(glm::vec3(this->chunk_x+x, this->chunk_y+y, this->chunk_z+z));
                }
            }
        }
    }
}
