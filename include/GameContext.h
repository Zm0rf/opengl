#pragma once

class GameContext;
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "World.h"
#include "Camera.h"
#include "graphics/Shader.h"
#include "Actor.h"

/**
 * TODO remove logic from this class
 * TODO make Game friend class, allowing only Game to alter?
 */
class GameContext
{
    public:
        GameContext(): scroll_wheel(0) {};
        ~GameContext() {};
    public:
        Actor* main_actor;
        Camera camera;

        // TODO move
        double scroll_wheel;
        // TODO move the shader to a grapihcs/render/model-related context.
        Shader* shader;
        double time_now, time_last_frame, time_delta;
        World* world;

        // Create sub-object for graphics related matrices...
        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;

        // TODO where should the glfw related variables reside? input/??.??
        GLFWwindow* window;
        double mouse_x, mouse_y;
        int window_width, window_height;

        // TODO move.
        bool do_stop;

        // (blocks/second)
        float movement_speed;
};
