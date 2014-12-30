#include "main.h"


int main(void)
{
    glfwSetErrorCallback([](int error_code, const char* error_description)
            {
                fprintf(stderr, "GLFW error (%2$d): %1$s\n", error_description, error_code);
            });
    Game game;
    game.mainLoop();
    return 0;
}

void updatePhysics(GameContext* context) // Look in misc.h if you are looking for me!
{
    context->main_actor->velocity.y -= context->time_delta * 0.8f;

    WorldChunk* chunk = context->world->getChunkAt(glm::vec3(0.0f, 0.0f, 0.0f));
    chunk->checkCollides(context);
    chunk = context->world->getChunkAt(glm::vec3(1.0f, 0.0f, 0.0f));
    chunk->checkCollides(context);

    context->main_actor->position += context->main_actor->velocity * (float)(context->time_delta * VELOCITY_INCREMENT);

    // Make the player not fall through the world when we have no block (or chunk) beneath him/her
    if( context->main_actor->position.y <= 0 )
    {
        context->main_actor->position.y = 0;
        context->main_actor->velocity.y = 0;
    }
}

