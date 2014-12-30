#pragma once

#include "GameContext.h"
#include "InputHandler.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp> // for glm::to_string


void onWindowResize(GameContext* context, GLFWwindow* window, int width, int height);
/**
 */
class Game
{
    private:
        bool main_running;
        GameContext context;
        World world;
        InputHandler input_handler;
    public:
        /** TODO move somewhere!?  */
        static void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);

        Game();
        ~Game();
        GameContext* getContext();
        void mainLoop();
};

extern Game game;
