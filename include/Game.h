#pragma once

#include "GameContext.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


void onWindowResize(GameContext* context, GLFWwindow* window, int width, int height);
/**
 */
class Game
{
    private:
        bool main_running;
        GameContext context;
    public:

        static void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);
        Game();
        ~Game();
        GameContext* getContext();
        void mainLoop();
};

extern Game game;
