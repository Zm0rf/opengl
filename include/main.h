// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std; // TODO remove using.

// Include GLFW
#include <GLFW/glfw3.h>

// Include glm
// - Force radius to remove deprecated behavour using degrees for rotation.
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "cube_data.h"

#define PI 3.1415
#define degreesToRadians(deg) deg*(PI/180.0f)
#define MOVE_INCREMENT 0.1f;

void renderCube(glm::vec3 position);

#include "Shader.h"

