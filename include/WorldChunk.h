#pragma once

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <math.h>

class WorldChunk;
#include "Actor.h"
#include "GameContext.h"

// TODO remove
void renderCube(
        glm::vec3 position,
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 origo = glm::vec3(0.0f));

#define CHUNK_SIZE 20
class WorldChunk
{
    public:
        WorldChunk();
        ~WorldChunk();
        void render();
        void checkCollides(GameContext* context);
        /* glm::vec3 foo(glm::vec3 point); */

    private:
        // TODO better datatype (object?)
        bool data[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
        int chunk_x;
        int chunk_y; // salsa
        int chunk_z;
        std::vector<Actor> actor_list;
};
