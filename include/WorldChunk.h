#pragma once

#include <stdio.h>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>

class WorldChunk;
#include "Actor.h"
#include "GameContext.h"

// TODO remove
void renderCube(glm::vec3 position);

#define CHUNK_SIZE 20
class WorldChunk
{
    public:
        WorldChunk();
        ~WorldChunk();
        void render();
        void checkCollides(GameContext* context);
        // void checkCollisions();

    private:
        // TODO better datatype (object?)
        bool data[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
        int chunk_x;
        int chunk_y; // salsa
        int chunk_z;
        std::vector<Actor> actor_list;
};
