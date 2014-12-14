#include "World.h"

World::World()
{
    // Beware of snö.
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
