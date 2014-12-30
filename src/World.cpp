#include "World.h"

World::World():
    x(0), y(0), z(0),
    cx(WORLD_CACHE_SIZE/2), cy(WORLD_CACHE_SIZE/2), cz(WORLD_CACHE_SIZE/2)
{
    // Beware of snö(plog?).
    for( int x=0; x<WORLD_CACHE_SIZE; x++ )
    {
        for( int y=0; y<WORLD_CACHE_SIZE; y++ )
        {
            for( int z=0; z<WORLD_CACHE_SIZE; z++ )
            {
                this->chunk_list[x][y][z] = new WorldChunk(x, y-1, z);
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
    // Get the chunk position of the actor.
    long x = floor(this->target_actor->position.x / CHUNK_SIZE);
    long y = floor(this->target_actor->position.y / CHUNK_SIZE);
    long z = floor(this->target_actor->position.z / CHUNK_SIZE);

    // Check if the center chunk still is in the center.
    WorldChunk* middle_chunk = this->getChunkAt(glm::vec3(
                this->cx,
                this->cy,
                this->cz));

    long xdiff = x - middle_chunk->chunk_x/CHUNK_SIZE;
    long ydiff = y - middle_chunk->chunk_y/CHUNK_SIZE;
    long zdiff = z - middle_chunk->chunk_z/CHUNK_SIZE;
    if( xdiff == 0 && ydiff == 0 && zdiff == 0 ) return;

    // Update what chunk should be the center chunk.
    this->cx = (this->cx + xdiff) % WORLD_CACHE_SIZE;
    this->cy = (this->cy + ydiff) % WORLD_CACHE_SIZE;
    this->cz = (this->cz + zdiff) % WORLD_CACHE_SIZE;
    // (clamp index to the cache size)
    if( this->cx < 0 ) this->cx += WORLD_CACHE_SIZE;
    if( this->cy < 0 ) this->cy += WORLD_CACHE_SIZE;
    if( this->cz < 0 ) this->cz += WORLD_CACHE_SIZE;

    // Update the existing chunks.
    printf( "World update(): "
            "x%ld y%ld z%ld \t"
            "dx%ld dy%ld dz%ld \t"
            "cx%d cy%d cz%d \n",
            x, y, z,
            xdiff, ydiff, zdiff,
            this->cx, this->cy, this->cz
            );

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
void World::setTargetActor(Actor* target_actor)
{
    this->target_actor = target_actor;
}
