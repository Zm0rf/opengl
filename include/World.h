#pragma once

#include "WorldChunk.h"

class World
{
    public:
        World();
        ~World();
        WorldChunk* getChunkAt(glm::vec3 pos);
    private:
        WorldChunk tmp_chunk;
};
