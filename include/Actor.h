#pragma once

#include <glm/glm.hpp>

class Actor
{
    private:
    public:
        float radius;
        float height;
        glm::vec3 velocity;
        glm::vec3 position;
        glm::vec3 rotation;

        Actor():
            radius(1.0f), height(1.5f),
            velocity(0.0f),
            position(10.0f, 4.0f, 10.0f),
            rotation(0.0f)
        {};
};
