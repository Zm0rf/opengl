#include "Renderer.h"

Renderer::Renderer():
    shader("data/shader/SimpleVertexShader.vertexshader", "data/shader/SimpleFragmentShader.fragmentshader")
{
    glGenVertexArrays(1, &this->vertex_array_id);
    glBindVertexArray(this->vertex_array_id);

    if( !this->shader.link() )
    {
        fprintf(stderr, "Failed to initialize main shader\n");
        exit(1);
    }
}
Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &this->vertex_array_id);
}

void Renderer::render(GameContext* context)
{
    glUseProgram(this->shader.getProgramId());
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
void Renderer::prepareRender(GameContext* context)
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

    GLuint test_buffer;
    glGenBuffers(1, &test_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, test_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_raw_data), cube_raw_data, GL_STATIC_DRAW);

    // Bind shader locations.
    GLuint shader_id = this->shader.getProgramId();
    glBindAttribLocation(shader_id, ATTRIB_VERTEX_POSITION_LOC, ATTRIB_VERTEX_POSITION_NAME);
    glBindAttribLocation(shader_id, ATTRIB_VERTEX_COLOR_LOC,    ATTRIB_VERTEX_COLOR_NAME);
    glBindAttribLocation(shader_id, ATTRIB_VERTEX_TEXCOORD_LOC, ATTRIB_VERTEX_TEXCOORD_NAME);

    // Setup cube data
    glGenBuffers(1, &this->render_data.vertex_buffer);
    glGenBuffers(1, &this->render_data.texcoord_buffer);
    glGenBuffers(1, &this->render_data.color_buffer);
    //
    glBindBuffer(GL_ARRAY_BUFFER, this->render_data.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    //
    glBindBuffer(GL_ARRAY_BUFFER, this->render_data.texcoord_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_texture_coordinates), g_texture_coordinates, GL_STATIC_DRAW);
    //
    glBindBuffer(GL_ARRAY_BUFFER, this->render_data.color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(ATTRIB_VERTEX_POSITION_LOC);
    glEnableVertexAttribArray(ATTRIB_VERTEX_COLOR_LOC);
    glEnableVertexAttribArray(ATTRIB_VERTEX_TEXCOORD_LOC);

    // Prepare rendering
    glBindBuffer(GL_ARRAY_BUFFER, test_buffer);
    glVertexAttribPointer(ATTRIB_VERTEX_POSITION_LOC, 3,    GL_FLOAT, GL_FALSE,   5*sizeof(GLfloat),      (void*)0);

    glVertexAttribPointer(ATTRIB_VERTEX_COLOR_LOC,    3,    GL_FLOAT, GL_FALSE,   0,      (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, test_buffer);
    glVertexAttribPointer(ATTRIB_VERTEX_TEXCOORD_LOC, 2,    GL_FLOAT, GL_FALSE,   5*sizeof(GLfloat),      (void*) (3*sizeof(GLfloat)));

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
