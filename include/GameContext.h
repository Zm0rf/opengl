#pragma once

#include <GLFW/glfw3.h>
#include "World.h"

class GameContext
{
    public:
        GLFWwindow* window;
        double time_delta;
        double time_last_frame;
        double time_now;
        int width;
        int height;
        World world;
};

