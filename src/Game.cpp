#include "Game.h"

Game::Game()
{
}
Game::~Game()
{
}
void Game::glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    onWindowResize(&game.context, game.context.window, game.context.window_width, game.context.window_height);
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
    glViewport(0, 0, context->window_width, context->window_height);
}
