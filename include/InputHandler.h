#pragma once

#include "GameContext.h"

/**
 * TODO break out keyboard management?
 */
class InputHandler
{
    // Static
    private:
        static InputHandler* active_handler;
    public:
        static InputHandler* activate(InputHandler* handler);
        static InputHandler* getActiveHandler();
        //
        static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void glfwMouseScrollCallback(GLFWwindow* window, double x, double y);

    // Non static
    private:
        void onActivate();
        void onDeactivate();
    public:
        InputHandler(): context(NULL){};
        GameContext* context;
        void onGlfwKey(GLFWwindow* window, int key, int scancode, int action, int mods);
        void onGlfwMouseScroll(GLFWwindow* window, double x, double y);
};
