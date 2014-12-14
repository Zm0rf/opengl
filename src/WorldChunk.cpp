#include "WorldChunk.h"

WorldChunk::WorldChunk():
    chunk_x(0), chunk_y(0), chunk_z(0)
{
    for( int x=0; x<CHUNK_SIZE; x++ )
    {
        for( int y=0; y<CHUNK_SIZE; y++ )
        {
            for( int z=0; z<CHUNK_SIZE; z++ )
            {
                this->data[x][y][z] = false;
                if( (x+z)/2 == y )
                    this->data[x][y][z] = true;
            }
        }
    }
}
WorldChunk::~WorldChunk()
{
}
void WorldChunk::render()
{
    for( int x=0; x<CHUNK_SIZE; x++ )
    {
        for( int y=0; y<CHUNK_SIZE; y++ )
        {
            for( int z=0; z<CHUNK_SIZE; z++ )
            {
                if( this->data[x][y][z] )
                {
                    renderCube(glm::vec3(this->chunk_x+x, this->chunk_y+y, this->chunk_z+z));
                }
            }
        }
    }
}
/* glm::vec3 WorldChunk::foo(glm::vec3 point) */
/* { */
/*     int x = (int)point.x; */
/*     int y = (int)point.y; */
/*     int z = (int)point.z; */
/*     // printf("%d %d %d\n", x, y, z); */
/*     // */
/*     if( x >= CHUNK_SIZE || x < 0 */
/*         || y >= CHUNK_SIZE || y < 0 */
/*         || z >= CHUNK_SIZE || z < 0 ) */
/*     { */
/*         return glm::vec3(0.0f); */
/*     } */
/*     if( this->data[x][y][z] ) */
/*     { */
/*         float xd = (point.x - x) + 0.5f; */
/*         float zd = (point.z - z) + 0.5f; */
/*         float yd = (point.y - y) + 0.5f; */
/*         /1* if( xd > zd ) *1/ */
/*         /1*     xd = 0.0f; *1/ */
/*         /1* else *1/ */
/*         /1*     zd = 0.0f; *1/ */
/*         /1* printf("COLISION %f %f %f\n", xd, zd, yd); *1/ */
/*         /1* yd=0; *1/ */
/*         /1* context->main_actor->velocity = glm::vec3(xd, yd, zd) * 0.1f; *1/ */
/*         /1* context->main_actor->velocity.y = 0; *1/ */
/*         /1* return glm::vec4; *1/ */
/*         return glm::vec3(xd, yd, zd); */
/*     } */
/*     return glm::vec3(0.0); */
/* } */
void WorldChunk::checkCollides(GameContext* context)
{
    // Get colliding block location
    // Calculate collision normal vector by getting the normal for each possible collission point.
    // Decrement velocity with the normal vector to prevent 

    // TODO remove following (dummy code)
    glm::vec3 point = context->main_actor->position;
    int x = (int)point.x;
    int y = (int)point.y;
    int z = (int)point.z;
    //
    if(    x >= CHUNK_SIZE || x < 0
        || y >= CHUNK_SIZE || y < 0
        || z >= CHUNK_SIZE || z < 0 )
    {
        return;
    }
    if( this->data[x][y][z] )
    {
        float xd = (point.x - x) + 0.5f;
        float zd = (point.z - z) + 0.5f;
        float yd = (point.y - y) + 0.5f;
        if( context->main_actor->velocity.y < 0 )
            context->main_actor->velocity.y = 0;
    }
}
