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
    /* Shader s("data/shader/font_shader.vert", "data/shader/font_shader.frag"); */
    if( !s.link() )
    {
        fprintf(stderr, "Failed to initialize main shader\n");
        return -1;
    }
    GLuint programID = s.getProgramId();

    /* Shader gui_shader("data/shader/GuiShader.vert", "data/shader/GuiShader.frag"); */
    /* Shader gui_shader("data/shader/GuiShader.vert", "data/shader/SimpleFragmentShader.fragmentshader"); */
    /* gui_shader.link(); */

	//Create vertex buffer
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//Create texture coordinate buffer
	GLuint texcoordbuffer;
	glGenBuffers(1, &texcoordbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_texture_coordinates), g_texture_coordinates, GL_STATIC_DRAW);


	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	//GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ModelID = glGetUniformLocation(programID, "M");
	GLuint ViewID = glGetUniformLocation(programID, "V");
	GLuint ProjectionID = glGetUniformLocation(programID, "P");

    WorldChunk chunk;

	glm::vec3 cameraPosition(0,0,0);
	glm::vec3 cameraRotation(0,0,0);
    glm::vec3 velocity(0,0,0);
	double mouseY, mouseX;

    int png_width, png_height;
    GLuint font_texture = png_texture_load("data/font/monospaced_bold.png", &png_width, &png_height);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

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
		//
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordbuffer);
		glVertexAttribPointer(
			3,
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
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

        glBindSampler( glGetUniformLocation(programID, "test_texture"), font_texture);

        // Make the cubes move :D
        float pos = 3.0f;
        pos += cos(context.time_now*2.0f);
        pos += cos(context.time_now*5.0f)*0.3;
        if( glfwGetKey(context.window, GLFW_KEY_E) == GLFW_PRESS )
        {
            pos += cos(context.time_now*100.0f)*0.04;
        }


        // Render GUI
        /* glEnable(GL_BLEND); */
        /* glEnable(GL_DEPTH_TEST); */
        /* glUseProgram(gui_shader.getProgramId()); */
        /* /1* glUniform2f(glGetUniformLocation(gui_shader.getProgramId(), "screen_dimensions"), context.width, context.height); *1/ */
        /* renderCube(glm::vec3(0.0f, 0.0f, 0.0f)); */


        // Rendering
        glUseProgram(programID);
        renderCube(glm::vec3(pos, 0.0f, 0.0f));
        renderCube(glm::vec3(-pos, 0.0f, 0.0f));
        renderCube(glm::vec3(0.0f, pos, 0.0f));
        renderCube(glm::vec3(0.0f, -pos, 0.0f));
        renderCube(glm::vec3(0.0f, 0.0f, pos));
        renderCube(glm::vec3(0.0f, 0.0f, -pos));

        chunk.render();

        // Main loop cleanup

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(context.window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (
            (glfwGetKey(context.window, GLFW_KEY_ESCAPE) != GLFW_PRESS) &&
            (glfwWindowShouldClose(context.window) == 0) );

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

    // Do not draw the faces that is pointed away from the camera
    /* glEnable(GL_CULL_FACE); */
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /* glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR); */
    /* glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA); */

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

GLuint png_texture_load(const char * file_name, int * width, int * height)
{
    // This is "borrowed" from the intertubes.. we should refactor and read what it does.
    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    if (width){ *width = temp_width; }
    if (height){ *height = temp_height; }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = (png_byte*)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = (png_bytep*)malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    int i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_width, temp_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return texture;
}
