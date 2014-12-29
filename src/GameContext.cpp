#include "GameContext.h"

GameContext::GameContext()
{
    this->world = new World();
    this->movement_speed = 4.0f;

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Set GLFW Major version to OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Set GLFW Minor version to OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window_width = 1024;
    this->window_height = 768;

	this->window = glfwCreateWindow(
            this->window_width, this->window_height,
            "OPENGL TEST :D",
            NULL, NULL);

    if( this->window == NULL )
    {
        fprintf(stderr, "Failed to create GLFW window.\n");
        exit(1);
    }

    /* glfwSetWindowSizeCallback(this->window, [](GLFWwindow* window, int width, int height) */
    /*         { */
    /*             onWindowResize(this, window, width, height); */
    /*         }); */

	if (this->window == NULL)
    {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
        exit(1);
	}
	glfwMakeContextCurrent(this->window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
    {
		fprintf(stderr, "Failed to initialize GLEW\n");
        exit(1);
	}
    /* printf("# Flushing GL error for invalid enumerant.. (bug in GLEW)\n"); */
    /* nagGlErrors(); */

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GL_TRUE);

    //onWindowResize(this, this->window, this->window_width, this->window_height);
}
