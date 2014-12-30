#pragma once

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <math.h>

class WorldChunk;
#include "Actor.h"
#include "Renderer.h"
#include "GameContext.h"


class BlockData
{
    public:
        bool passable;
};
class Block
{
    public:
        Block(): data(NULL){};
        bool isValid() { return this->data != NULL; }
        glm::vec3 toVec() { return glm::vec3(this->x, this->y, this->z); }

        int x, y, z;
        BlockData* data;
};

#define CHUNK_SIZE 20
class WorldChunk
{
    public:
        WorldChunk();
        WorldChunk(long x, long y, long z);
        ~WorldChunk();
        void render();
        void checkCollides(GameContext* context);
        Block getBlockAt(glm::vec3 pos);

        long chunk_x;
        long chunk_y; // salsa
        long chunk_z;
    private:
        void initDummyData();
        // TODO better datatype (object?)
        BlockData data[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
        std::vector<Actor> actor_list;
};
