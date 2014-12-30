#include "InputHandler.h"

InputHandler* InputHandler::active_handler = NULL;

/* void InputHandler::activate() */
/* { */
/*     glfwSetKeyCallback(context->window, InputHandler::glfwKeyCallback); */
/* } */

InputHandler* InputHandler::activate(InputHandler* handler)
{
    InputHandler* previous = InputHandler::active_handler;
    InputHandler::active_handler = handler;
    if( previous != NULL ) previous->onDeactivate();
    if( InputHandler::active_handler != NULL ) InputHandler::active_handler->onActivate();
    return previous;
}
InputHandler* InputHandler::getActiveHandler()
{
    return InputHandler::active_handler;
}
void InputHandler::onActivate()
{
    glfwSetKeyCallback(this->context->window, InputHandler::glfwKeyCallback);
    glfwSetScrollCallback(context->window, InputHandler::glfwMouseScrollCallback);
}
void InputHandler::onDeactivate()
{
    glfwSetKeyCallback(this->context->window, NULL);
}
void InputHandler::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputHandler* handler = InputHandler::getActiveHandler();
    if( handler == NULL )
        return;
    handler->onGlfwKey(window, key, scancode, action, mods);
}
void InputHandler::glfwMouseScrollCallback(GLFWwindow* window, double x, double y)
{
    InputHandler* handler = InputHandler::getActiveHandler();
    if( handler == NULL )
        return;
    handler->onGlfwMouseScroll(window, x, y);
}

void InputHandler::onGlfwKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if( this->context == NULL )
        return;
    if( action == GLFW_PRESS )
        return;
    switch( key )
    {
        case GLFW_KEY_ESCAPE:
            // Halt the program
            this->context->do_stop = true;
            break;
        case GLFW_KEY_Q:
            // reset the game state
            this->context->main_actor->position = glm::vec3(0.0f);
            this->context->camera.rotation = glm::vec3(0.0f);
            break;
        case GLFW_KEY_C:
            // Clear the default color to see if any changes exists.
            glClearColor(0.0f, 0.1f, 0.0f, 0.0f);
            break;
        case GLFW_KEY_T:
            // Toggle moving slow and fast.
            if( this->context->movement_speed >= 4.0f )
                this->context->movement_speed = 0.4f;
            else
                this->context->movement_speed = 4.0f;
            break;
    }
}
void InputHandler::onGlfwMouseScroll(GLFWwindow* window, double x, double y)
{
    this->context->scroll_wheel += y;
    if( this->context->scroll_wheel < 0 )
        this->context->scroll_wheel = 0;
}
void manageUserInput(GameContext* context)
{
    // Actor rotation
    glfwGetCursorPos(context->window, &context->mouse_x, &context->mouse_y);
    glfwSetCursorPos(context->window, context->window_width/2, context->window_height/2);
    context->camera.rotation.y -= (context->mouse_x - context->window_width/2)*0.01f;
    context->camera.rotation.x += INVERT_MOUSE_MODIFIER * (context->mouse_y - context->window_height/2)*0.01f;

    // Clamp the rotation not to loop
    if( context->camera.rotation.x < -0.5f*PI )
    {
        context->camera.rotation.x = -0.5f*PI;
    }
    else if( context->camera.rotation.x > 0.5f*PI )
    {
        context->camera.rotation.x = 0.5f*PI;
    }

    // Actor movement
    glm::vec3 dist;
    if (glfwGetKey(context->window, GLFW_KEY_W))
    {
        dist.z -= context->time_delta * context->movement_speed;
    }
    if (glfwGetKey(context->window, GLFW_KEY_S))
    {
        dist.z += context->time_delta * context->movement_speed;
    }
    if (glfwGetKey(context->window, GLFW_KEY_A))
    {
        dist.x -= context->time_delta * context->movement_speed;
    }
    if (glfwGetKey(context->window, GLFW_KEY_D))
    {
        dist.x += context->time_delta * context->movement_speed;
    }
    if( glfwGetKey(context->window, GLFW_KEY_SPACE) && context->main_actor->velocity.y == 0 )
    {
        context->main_actor->velocity.y = 0.2f;
    }
    dist = glm::rotate(dist, context->camera.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    context->main_actor->velocity.x = dist.x;
    context->main_actor->velocity.z = dist.z;

    // Rotate the actor.. TODO move this.
    context->main_actor->rotation = glm::vec3(
            0.0f,
            context->camera.rotation.y,
            0.0f);
}
