#pragma once

class World;
#include "WorldChunk.h"

#include <stdlib.h>

// Note that the WORLD_CACHE_SIZE should be an odd number (the player will be in the middle chunk..)
#define WORLD_CACHE_SIZE 3

class World
{
    public:
        World();
        ~World();
        WorldChunk* getChunkAt(glm::vec3 pos);
        void update();
        void render();

        void setTargetActor(Actor* target_actor);
    private:
        long x, y, z;
        int cx, cy, cz;
        WorldChunk* tmp_chunk;
        WorldChunk* chunk_list[WORLD_CACHE_SIZE][WORLD_CACHE_SIZE][WORLD_CACHE_SIZE];
        Actor* target_actor;

};
