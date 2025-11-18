#ifndef BOID_H
#define BOID_H

#include "game_object.h"
#include "texture.h"
#include <glm/glm.hpp>
#include "../graphics/sprite_renderer.h"

class BoidObject : public GameObject
{
    public:
        float radius;
        float rotation;
        float visionRadius;

        BoidObject();
        BoidObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite, float visionRadius);

        glm::vec2 move(float dt, unsigned int window_width, unsigned int window_height);
        void reset(glm::vec2 position, glm::vec2 velocity);
        void draw(SpriteRenderer &renderer);
}; 

#endif