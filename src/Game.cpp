#include "Game.h"

Game::Game()
{
    this->context.world = &this->world;
    this->context.movement_speed = 4.0f;

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

    this->context.window_width = 1024;
    this->context.window_height = 768;

    // Create GLFW window.
    this->context.window = glfwCreateWindow(
            this->context.window_width, this->context.window_height,
            "OPENGL TEST :D",
            NULL, NULL);

    if (this->context.window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(this->context.window);

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
	glfwSetInputMode(this->context.window, GLFW_STICKY_KEYS, GL_TRUE);

    this->input_handler.context = &this->context;
    InputHandler::activate(&this->input_handler);
}

Game::~Game()
{
}
void Game::glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    onWindowResize(&game.context, window, width, height);
}
GameContext* Game::getContext()
{
    return &(this->context);
}
void Game::mainLoop()
{
}
void onWindowResize(GameContext* context, GLFWwindow* window, int width, int height)
{
    context->window_width = width;
    context->window_height = height;
    context->projection_matrix = glm::perspective(
            45.0f,
            /* 4.0f / 3.0f, */
            (float)context->window_width / (float)context->window_height,
            0.1f,
            100.0f);
    printf("%d %d\n", width, height);
    glViewport(0, 0, context->window_width, context->window_height);
}
