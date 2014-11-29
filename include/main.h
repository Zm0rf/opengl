
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
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
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

//
#include "cube_data.h"

#define PI 3.1415
#define degreesToRadians(deg) deg*(PI/180.0f)
#define MOVE_INCREMENT 0.1f;

void renderCube(glm::vec3 position);

#include "Shader.h"

#define CHUNK_SIZE 10
class WorldChunk
{
    public:
        WorldChunk();
        ~WorldChunk();
        void render();
    private:
        // TODO better datatype (object?)
        bool data[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
        int chunk_x;
        int chunk_y; // salsa
        int chunk_z;
};

class GameContext
{
    public:
        GLFWwindow* window;
        double time_delta;
        double time_last_frame;
        double time_now;
        int width;
        int height;
};

bool initContext(GameContext* context);

void onWindowResize(GLFWwindow* window, int width, int height);
