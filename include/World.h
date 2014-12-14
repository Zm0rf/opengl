#pragma once

#include <stdlib.h>
class World;
#include "WorldChunk.h"

#define WORLD_CACHE_SIZE 3

class World
{
    public:
        World();
        ~World();
        WorldChunk* getChunkAt(glm::vec3 pos);
    private:
        WorldChunk* tmp_chunk;
        WorldChunk* chunk_list[WORLD_CACHE_SIZE][WORLD_CACHE_SIZE][WORLD_CACHE_SIZE];
};
