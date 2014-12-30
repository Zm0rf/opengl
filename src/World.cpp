#include "World.h"

World::World():
    x(0), y(0), z(0)
{
    // Beware of snö(plog?).
    for( int x=0; x<WORLD_CACHE_SIZE; x++ )
    {
        for( int y=0; y<WORLD_CACHE_SIZE; y++ )
        {
            for( int z=0; z<WORLD_CACHE_SIZE; z++ )
            {
                this->chunk_list[x][y][z] = new WorldChunk(x, y, z);
            }
        }
    }
}
World::~World()
{
    for( int x=0; x<WORLD_CACHE_SIZE; x++ )
    {
        for( int y=0; y<WORLD_CACHE_SIZE; y++ )
        {
            for( int z=0; z<WORLD_CACHE_SIZE; z++ )
            {
                if( this->chunk_list[x][y][z] != NULL )
                    delete this->chunk_list[x][y][z];
            }
        }
    }
}
WorldChunk* World::getChunkAt(glm::vec3 pos)
{
    int x = (int)pos.x;
    int y = (int)pos.y;
    int z = (int)pos.z;
    if(    x >= WORLD_CACHE_SIZE || x < 0
        || y >= WORLD_CACHE_SIZE || y < 0
        || z >= WORLD_CACHE_SIZE || z < 0 )
    {
        return NULL;
    }
    return this->chunk_list[x][y][z];
}

void World::update()
{
}
void World::render()
{
    for( long i=0; i< WORLD_CACHE_SIZE; i++ )
    {
        for( long j=0; j< WORLD_CACHE_SIZE; j++ )
        {
            for( long k=0; k< WORLD_CACHE_SIZE; k++ )
            {
            WorldChunk* chunk = this->getChunkAt(glm::vec3(i, j, k));
            if( chunk == NULL ) continue;
            chunk->render();
            }
        }
    }
}
