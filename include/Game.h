#pragma once

#include "Renderer.h"
#include "GameContext.h"
#include "InputHandler.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp> // for glm::to_string



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
        Renderer renderer;

        Game();
        ~Game();
        GameContext* getContext();
        void mainLoop();
};
