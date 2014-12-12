#include "World.h"

World::World()
{
    this->tmp_chunk = new WorldChunk();
}
World::~World()
{
    delete this->tmp_chunk;
}
WorldChunk* World::getChunkAt(glm::vec3 pos)
{
    return this->tmp_chunk;
}
