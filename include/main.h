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
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp> // for glm::to_string

//
#include "Renderer.h"
#include "Game.h"
#include "InputHandler.h"
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

//
void manageUserInput(GameContext* context);
void updatePhysics(GameContext* context);
/**
 * Update GameContext time related data. (e.g. time_delta)
 */
void tick(GameContext* context);

void nagGlErrors();

