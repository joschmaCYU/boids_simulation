#ifndef BOID_H
#define BOID_H

#include "game_object.h"
#include "texture.h"
#include <glm/glm.hpp>

class BoidObject : public GameObject
{
    public:
        float radius;

        BoidObject();
        BoidObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

        glm::vec2 move(float dt, unsigned int window_width, unsigned int window_height);
        void reset(glm::vec2 position, glm::vec2 velocity);
}; 

#endif