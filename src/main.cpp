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
    onWindowResize(context, context->window, context->window_width, context->window_height);

    printf("# Flushing GL error for invalid enumerant.. (bug in GLEW)\n");
    nagGlErrors();


    InputHandler input_handler;
    input_handler.context = context;
    InputHandler::activate(&input_handler);
    InputHandler::getActiveHandler()->context = context;


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    Shader shader_program("data/shader/SimpleVertexShader.vertexshader", "data/shader/SimpleFragmentShader.fragmentshader");
    /* Shader s("data/shader/font_shader.vert", "data/shader/font_shader.frag"); */
    if( !shader_program.link() )
    {
        fprintf(stderr, "Failed to initialize main shader\n");
        return -1;
    }
    Actor main_actor;
    context->main_actor = &main_actor;
    context->shader = &shader_program;
    //
    context->main_actor->velocity = glm::vec3(0,0,0);
    context->main_actor->position = glm::vec3(10.0f, 4.0f, 10.0f);

    RenderData render_data;
    prepareRender(context, &render_data);

    context->do_stop = false;
    while( !context->do_stop || glfwWindowShouldClose(context->window) )
    {
        tick(context);
        manageUserInput(context);
        updatePhysics(context);
        render(context);

        // Swap buffers
        glfwSwapBuffers(context->window);
        glfwPollEvents();
        // Error handling
        nagGlErrors();
	}

	// Cleanup VBO
	glDeleteBuffers(1, &render_data.vertex_buffer);
	glDeleteVertexArrays(1, &VertexArrayID);

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

void render(GameContext* context)
{
    glUseProgram(context->shader->getProgramId());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    setupCamera(context);

    glm::mat4 View = glm::mat4(1.0f);
    View = glm::rotate(View, context->camera.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, context->camera.rotation.y, -glm::vec3(0.0f, 1.0f, 0.0f));
    View = glm::translate(View, -context->camera.position);
    glm::mat4 projection_view = context->projection_matrix * View;

    glUniformMatrix4fv(UNIFORM_PROJECTION_VIEW_LOC, 1, GL_FALSE, &projection_view[0][0]);

    // Render the player..
    renderCube(
            context->main_actor->position,
            context->main_actor->rotation,
            -glm::vec3(0.5f, 0.0f, 0.5f));

    tmpRenderMovingCubes(context, glm::vec3(10.0f, 20.0f, 10.0f));
    for( long i=0; i< WORLD_CACHE_SIZE; i++ )
    {
        for( long j=0; j< WORLD_CACHE_SIZE; j++ )
        {
            WorldChunk* chunk = context->world->getChunkAt(glm::vec3(i, 0.0f, j));
            if( chunk == NULL )
                continue;
            chunk->render();
        }
    }
    /* context->world->getChunkAt(glm::vec3(0.0f, 0.0f, 0.0f))->render(); */

    // TODO is this needed?
    /* glDisableVertexAttribArray(0); */
}
void prepareRender(GameContext* context, RenderData* render_data)
{
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Do not draw the faces that is pointed away from the camera
    /* glEnable(GL_CULL_FACE); */
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /* glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR); */
    /* glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA); */

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable vsync
    glfwSwapInterval(1);
    /* Shader gui_shader("data/shader/GuiShader.vert", "data/shader/GuiShader.frag"); */
    /* Shader gui_shader("data/shader/GuiShader.vert", "data/shader/SimpleFragmentShader.fragmentshader"); */
    /* gui_shader.link(); */


    GLuint test_buffer;
    glGenBuffers(1, &test_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, test_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_raw_data), cube_raw_data, GL_STATIC_DRAW);

    // Bind shader locations.
    glBindAttribLocation(context->shader->getProgramId(), ATTRIB_VERTEX_POSITION_LOC, ATTRIB_VERTEX_POSITION_NAME);
    glBindAttribLocation(context->shader->getProgramId(), ATTRIB_VERTEX_COLOR_LOC,    ATTRIB_VERTEX_COLOR_NAME);
    glBindAttribLocation(context->shader->getProgramId(), ATTRIB_VERTEX_TEXCOORD_LOC, ATTRIB_VERTEX_TEXCOORD_NAME);

    // Setup cube data
    glGenBuffers(1, &render_data->vertex_buffer);
    glGenBuffers(1, &render_data->texcoord_buffer);
    glGenBuffers(1, &render_data->color_buffer);
    //
    glBindBuffer(GL_ARRAY_BUFFER, render_data->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    //
    glBindBuffer(GL_ARRAY_BUFFER, render_data->texcoord_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_texture_coordinates), g_texture_coordinates, GL_STATIC_DRAW);
    //
    glBindBuffer(GL_ARRAY_BUFFER, render_data->color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(ATTRIB_VERTEX_POSITION_LOC);
    glEnableVertexAttribArray(ATTRIB_VERTEX_COLOR_LOC);
    glEnableVertexAttribArray(ATTRIB_VERTEX_TEXCOORD_LOC);

    // Prepare rendering
    /* glBindBuffer(GL_ARRAY_BUFFER, render_data->vertex_buffer); */
    glBindBuffer(GL_ARRAY_BUFFER, test_buffer);
    /* glVertexAttribPointer(ATTRIB_VERTEX_POSITION_LOC, 3,    GL_FLOAT, GL_FALSE,   0,      (void*)0); */
    glVertexAttribPointer(ATTRIB_VERTEX_POSITION_LOC, 3,    GL_FLOAT, GL_FALSE,   5*sizeof(GLfloat),      (void*)0);

    /* glBindBuffer(GL_ARRAY_BUFFER, render_data->color_buffer); */
    /* glVertexAttrib3f(ATTRIB_VERTEX_COLOR_LOC, 1.0f, 0.0f, 0.0f); */
    glVertexAttribPointer(ATTRIB_VERTEX_COLOR_LOC,    3,    GL_FLOAT, GL_FALSE,   0,      (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, test_buffer);
    /* glBindBuffer(GL_ARRAY_BUFFER, render_data->texcoord_buffer); */
    /* glVertexAttribPointer(ATTRIB_VERTEX_TEXCOORD_LOC, 2,    GL_FLOAT, GL_FALSE,   0,      (void*)0); */
    glVertexAttribPointer(ATTRIB_VERTEX_TEXCOORD_LOC, 2,    GL_FLOAT, GL_FALSE,   5*sizeof(GLfloat),      (void*) (3*sizeof(GLfloat)));

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

GameContext::~GameContext()
{
    delete this->world;
}

void nagGlErrors()
{
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf("GL ERROR %s\n", gluErrorString(error));
    }
}
void setupCamera(GameContext* context)
{
    context->camera.position = context->main_actor->position;
    glm::vec3 head_bobbing = glm::vec3(0.0f);
    head_bobbing += glm::vec3(
            cos(context->time_now*5),
            sin(context->time_now*2*5),
            0.0f);
    head_bobbing *= glm::length(context->main_actor->velocity) * 7.0f;
    if( context->main_actor->velocity.y != 0.0f )
        head_bobbing = glm::vec3(0.0f);
    if( context->scroll_wheel > 0.0f )
    {
        head_bobbing *= 1.0f / context->scroll_wheel;
    }
    //
    glm::vec3 tmp = glm::vec3(0.0f);
    tmp += head_bobbing;
    tmp = glm::rotate(
            tmp + glm::vec3(0.0f, 0.0f, -1.0f) * (float)-context->scroll_wheel,
            -context->camera.rotation.x,
            glm::vec3(1.0f, 0.0f, 0.0f));
    tmp = glm::rotate(
            tmp + glm::vec3(0.0f, 2.0f, 0.0f),
            context->camera.rotation.y,
            glm::vec3(0.0f, 1.0f, 0.0f)
            );
    context->camera.position += tmp;
}
void tick(GameContext* context)
{
    // Update timing related stuff
    context->time_last_frame = context->time_now;
    context->time_now = glfwGetTime();
    context->time_delta = context->time_now - context->time_last_frame;
}
