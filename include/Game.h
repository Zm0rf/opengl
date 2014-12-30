#pragma once

#include "Renderer.h"
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
        GameContext context;
        World world;
        InputHandler input_handler;
        Actor main_actor;
    public:
        Renderer* renderer; // TODO make this private, non-pointer when this is not used in a global scope..
        /** TODO move somewhere!?  */
        static void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);

        Game();
        ~Game();
        GameContext* getContext();
        void mainLoop();
};

extern Game game; // TODO remove this
