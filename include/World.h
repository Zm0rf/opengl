#pragma once

class World;
#include "WorldChunk.h"

#include <stdlib.h>

#define WORLD_CACHE_SIZE 3

class World
{
    public:
        World();
        ~World();
        WorldChunk* getChunkAt(glm::vec3 pos);
        void update();
        void render();
    private:
        long x, y, z;
        WorldChunk* tmp_chunk;
        WorldChunk* chunk_list[WORLD_CACHE_SIZE][WORLD_CACHE_SIZE][WORLD_CACHE_SIZE];

};
