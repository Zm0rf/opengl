#pragma once

#include "graphics/Shader.h"
#include "GameContext.h"
#include "cube_data.h"

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

// Include glm
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp> // for glm::to_string

// Define global shader variables (TODO MOVE)
#define ATTRIB_VERTEX_POSITION_LOC 0
#define ATTRIB_VERTEX_POSITION_NAME "vertexPosition"
#define ATTRIB_VERTEX_COLOR_LOC 1
#define ATTRIB_VERTEX_COLOR_NAME "vertexColor"
#define ATTRIB_VERTEX_TEXCOORD_LOC 3
#define ATTRIB_VERTEX_TEXCOORD_NAME "vertexTexcoord"
#define UNIFORM_PROJECTION_VIEW_LOC 3

/**
 * Position and rotate camera.
 */
void setupCamera(GameContext* context);
void tmpRenderMovingCubes(GameContext* context, glm::vec3 pos);
void nagGlErrors();
// TODO remove
void renderCube(
        glm::vec3 position,
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 origo = glm::vec3(0.0f));

// TODO move and rethink
typedef struct {
    GLuint vertex_buffer;
    GLuint texcoord_buffer;
    GLuint color_buffer;
} RenderData;
/**
 * TODO rename
 */
class Renderer
{
    private:
        GLuint vertex_array_id;
        Shader shader;
    public:
        RenderData render_data; // TODO make private

        Renderer();
        ~Renderer();

        void render(GameContext* context);
        void prepareRender(GameContext* context);
};
