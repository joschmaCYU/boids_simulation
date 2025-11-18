#include "boid.h"


BoidObject::BoidObject() 
    : GameObject(), radius(12.5f), rotation(0.0f), visionRadius(25.0f) { }

BoidObject::BoidObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite, float visionRadius)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), radius(radius), rotation(0.0f), visionRadius(visionRadius) { }

glm::vec2 BoidObject::move(float dt, unsigned int window_width, unsigned int window_height)
{
    // Calculate angle to face direction of movement
    this->rotation = glm::degrees(glm::atan(this->velocity.y, this->velocity.x));

    this->position.x += this->velocity.x * dt;
    this->position.y += this->velocity.y * dt;
    // if outside window bounds comme back 
    if (this->position.x <= 0.0f) {
        this->position.x = window_width;
    } else if (this->position.x >= window_width) {
        this->position.x = 0.0f;
    }
    if (this->position.y <= 0.0f) {
        this->position.y = window_height;
    } else if (this->position.y >= window_height) {
        this->position.y = 0;
    }
    // move to mouse cursor

    // move in group

    return this->position;
}

// resets the ball to initial stuck position (if ball is outside window bounds)
void BoidObject::reset(glm::vec2 position, glm::vec2 velocity)
{
    this->position = position;
    this->velocity = velocity;
}

void BoidObject::draw(SpriteRenderer &renderer)
{
    renderer.drawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}