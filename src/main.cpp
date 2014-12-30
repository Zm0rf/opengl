#include "main.h"


Game game;
int main(void)
{
    glfwSetErrorCallback([](int error_code, const char* error_description)
            {
                fprintf(stderr, "GLFW error (%2$d): %1$s\n", error_description, error_code);
            });

    GameContext* context = game.getContext();

    glfwSetWindowSizeCallback(context->window, Game::glfwWindowSizeCallback);
    //onWindowResize(context, context->window, context->window_width, context->window_height);
    Game::glfwWindowSizeCallback(context->window, context->window_width, context->window_width);

    printf("# Flushing GL error for invalid enumerant.. (bug in GLEW)\n");
    nagGlErrors();

    Renderer renderer;

    // Create and compile our GLSL program from the shaders
    /* Shader shader_program("data/shader/SimpleVertexShader.vertexshader", "data/shader/SimpleFragmentShader.fragmentshader"); */
    /* Shader s("data/shader/font_shader.vert", "data/shader/font_shader.frag"); */
    /* if( !shader_program.link() ) */
    /* { */
    /*     fprintf(stderr, "Failed to initialize main shader\n"); */
    /*     return -1; */
    /* } */
    Actor main_actor;
    context->main_actor = &main_actor;
    /* context->shader = &shader_program; */
    //
    context->main_actor->velocity = glm::vec3(0,0,0);
    context->main_actor->position = glm::vec3(10.0f, 4.0f, 10.0f);

    renderer.prepareRender(context);

    context->do_stop = false;
    while( !context->do_stop || glfwWindowShouldClose(context->window) )
    {
        tick(context);
        manageUserInput(context);
        updatePhysics(context);
        renderer.render(context);

        // Swap buffers
        glfwSwapBuffers(context->window);
        glfwPollEvents();
        // Error handling
        nagGlErrors();
	}

	// Cleanup VBO
	glDeleteBuffers(1, &renderer.render_data.vertex_buffer);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
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


void updatePhysics(GameContext* context)
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


void renderCube(glm::vec3 position, glm::vec3 rotation, glm::vec3 origo)
{
	glm::mat4 Model = glm::mat4(1.0f);
		/* 1.0f, 0.0f, 0.0f, 0.0f, */
		/* 0.0f, 1.0f, 0.0f, 0.0f, */
		/* 0.0f, 0.0f, 1.0f, 0.0f, */
		/* position.x, position.y, position.z, 1.0f); */
    Model = glm::translate(Model, position);
    Model = glm::rotate(Model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, origo);
	glUniformMatrix4fv(2, 1, GL_FALSE, &Model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 3 indices starting at 0 -> 1 triangle
}

void tmpRenderMovingCubes(GameContext* context, glm::vec3 pos)
{
    float i = 3.0f;
    i += cos(context->time_now*2.0f);
    i += cos(context->time_now*5.0f)*0.3;

    // Rendering
    renderCube(pos+glm::vec3(i, 0.0f, 0.0f));
    renderCube(pos+glm::vec3(-i, 0.0f, 0.0f));
    renderCube(pos+glm::vec3(0.0f, i, 0.0f));
    renderCube(pos+glm::vec3(0.0f, -i, 0.0f));
    renderCube(pos+glm::vec3(0.0f, 0.0f, i));
    renderCube(pos+glm::vec3(0.0f, 0.0f, -i));
}

void nagGlErrors()
{
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf("GL ERROR %s\n", gluErrorString(error));
    }
}
void tick(GameContext* context)
{
    // Update timing related stuff
    context->time_last_frame = context->time_now;
    context->time_now = glfwGetTime();
    context->time_delta = context->time_now - context->time_last_frame;
}


