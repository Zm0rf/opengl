#pragma once

class GameContext;
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "World.h"
#include "graphics/Shader.h"

/**
 * TODO clean this up and make it a OBJECT and not a struct of data!
 */
class GameContext
{
    public:
        GameContext();
        ~GameContext();
    public:
        Shader* shader;
        GLFWwindow* window;
        double time_delta;
        double time_last_frame;
        double time_now;
        int width;
        int height;
        World* world;

        glm::vec3 camera_rotation;
        glm::vec3 camera_position;
        glm::vec3 velocity;

        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;

        double mouse_x, mouse_y;

        bool do_stop;
};
