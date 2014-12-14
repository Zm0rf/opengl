#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include glm
// - Force radius to remove deprecated behavour using degrees for rotation.
// #define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

//
#include "LinkedObjectFactory.h"
#include "cube_data.h"
#include "GameContext.h"
#include "WorldChunk.h"
#include "World.h"
#include "graphics/Shader.h"

// Helper stuff
#define PI 3.1415
#define degreesToRadians(deg) deg*(PI/180.0f)
// movement increment in meters (blocks) per second
#define VELOCITY_INCREMENT 200.0f

// TODO solve better (added this to mark where to enable inverted mouse)
#define INVERT_MOUSE_MODIFIER 1
// Define global shader variables (TODO MOVE)
#define ATTRIB_VERTEX_POSITION_LOC 0
#define ATTRIB_VERTEX_POSITION_NAME "vertexPosition"
#define ATTRIB_VERTEX_COLOR_LOC 1
#define ATTRIB_VERTEX_COLOR_NAME "vertexColor"
#define ATTRIB_VERTEX_TEXCOORD_LOC 3
#define ATTRIB_VERTEX_TEXCOORD_NAME "vertexTexcoord"
#define UNIFORM_PROJECTION_VIEW_LOC 3

// TODO move and rethink
typedef struct {
    GLuint vertex_buffer;
    GLuint texcoord_buffer;
    GLuint color_buffer;
} RenderData;

bool initContext(GameContext* context);

// GLFW callbacks
void onWindowResize(GLFWwindow* window, int width, int height);
void onMouseScroll(GLFWwindow* window, double x, double y);
/**
 * Handle key-presses that fires an action.
 * polling using glfwGetKey will be used for functions that better fitt polling.
 * e.g. WASD will be polled, open inventory will be abstracted to onKey
 */
void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);

//
void render(GameContext* context);
void manageUserInput(GameContext* context);
void updatePhysics(GameContext* context);
void tmpRenderMovingCubes(GameContext* context, glm::vec3 pos);
void prepareRender(GameContext* context, RenderData* render_data);
/**
 * Position and rotate camera.
 */
void setupCamera(GameContext* context);
/**
 * Update GameContext time related data. (e.g. time_delta)
 */
void tick(GameContext* context);

void nagGlErrors();
