#pragma once

#include <glm/glm.hpp>

class Actor
{
    public:
        Actor(): radius(1.0f), height(1.5f){};
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 velocity;
        float radius;
        float height;
    private:
};
