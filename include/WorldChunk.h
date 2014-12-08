#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Actor.h"

// TODO remove
void renderCube(glm::vec3 position);

#define CHUNK_SIZE 10
class WorldChunk
{
    public:
        WorldChunk();
        ~WorldChunk();
        void render();
        void checkCollisions();

    private:
        // TODO better datatype (object?)
        bool data[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
        int chunk_x;
        int chunk_y; // salsa
        int chunk_z;
        std::vector<Actor> actor_list;
};
