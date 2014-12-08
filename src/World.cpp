#include "World.h"

World::World()
{
}
World::~World()
{
}
WorldChunk* World::getChunkAt(glm::vec3 pos)
{
    return &this->tmp_chunk;
}
