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
                this->data[x][y][z].passable = false;
                if( (x+z)/2 == y )
                    this->data[x][y][z].passable = true;
                if( x == z && x == 12 && y == 0 )
                    this->data[x][y][z].passable = true;
                if( x == 15 && z == 0 )
                    this->data[x][y][z].passable = true;
                if( x > 15 && y == 0 )
                    this->data[x][y][z].passable = true;
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
                if( this->data[x][y][z].passable )
                {
                    renderCube(glm::vec3(this->chunk_x+x, this->chunk_y+y, this->chunk_z+z));
                }
            }
        }
    }
}
Block WorldChunk::getBlockAt(glm::vec3 pos)
{
    Block b;
    int x = (int)pos.x;
    int y = (int)pos.y;
    int z = (int)pos.z;
    if(    x >= CHUNK_SIZE || x < 0
        || y >= CHUNK_SIZE || y < 0
        || z >= CHUNK_SIZE || z < 0 )
    {
        return b;
    }
    b.data = &this->data[x][y][z];
    b.x = x;
    b.y = y;
    b.z = z;
    return b;
}
void WorldChunk::checkCollides(GameContext* context)
{
    Block b = this->getBlockAt(context->main_actor->position + context->main_actor->velocity);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    if( b.isValid() && b.data->passable)
    {
        glClearColor(0.3f, 0.0f, 0.0f, 0.0f);
        context->main_actor->position.y = this->chunk_y + b.y + 1.0f;
        context->main_actor->velocity.y = 0;
    }
    else
    {
        b = this->getBlockAt(
                context->main_actor->position +
                context->main_actor->velocity +
                glm::vec3(0.0f, context->main_actor->height, 0.0f));
        if( b.isValid() && b.data->passable )
        {
            glClearColor(0.3f, 0.2f, 0.0f, 0.0f);
            context->main_actor->position.y = this->chunk_y + b.y - context->main_actor->height;
            context->main_actor->velocity.y = -0.001f;
        }
    }
    std::vector<glm::vec3> pts = {
        glm::vec3(0.0f, 0.0f, 0.0f)
    };
}
