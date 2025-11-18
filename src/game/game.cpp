#include "game.h"
#include "../core/resource_manager.h"
#include "../graphics/sprite_renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>
#include <random>

// Game-related State data
SpriteRenderer  *renderer;

Game::Game(unsigned int width, unsigned int height) 
    : state(GAME_ACTIVE), keys(), width(width), height(height)
{ 

}

Game::~Game()
{
    delete renderer;
}

GameObject *player;


std::string vertShaderPath;
std::string fragShaderPath;
std::string texturePath;
std::string levelPath;

void Game::init()
{
    // Build paths relative to the executable location
    std::filesystem::path exePath = std::filesystem::current_path();
    std::filesystem::path projectRoot = exePath.parent_path(); // go from build/ to project root
    if (projectRoot.filename() == "build") {
        projectRoot = projectRoot.parent_path();
    }
    
    vertShaderPath = (projectRoot / "src" / "include" / "shaders" / "default.vert").string();
    fragShaderPath = (projectRoot / "src" / "include" / "shaders" / "default.frag").string();
    texturePath = (projectRoot / "src" / "texture").string();
    levelPath = (projectRoot / "src" / "levels").string();
    
    // load shaders
    ResourceManager::LoadShader(vertShaderPath.c_str(), fragShaderPath.c_str(), nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), 
        static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    
    // load textures
    ResourceManager::LoadTexture((texturePath + "/awesomeface.png").c_str(), true, "face");
    ResourceManager::LoadTexture((texturePath + "/background.jpg").c_str(), false, "background");
    ResourceManager::LoadTexture((texturePath + "/block.png").c_str(), false, "block");
    ResourceManager::LoadTexture((texturePath + "/block_solid.png").c_str(), false, "block_solid");
    ResourceManager::LoadTexture((texturePath + "/paddle.png").c_str(), true, "paddle");
    
    // load levels
    GameLevel menu;
    // menu.load((levelPath + "/welcome.lvl").c_str(), this-> width, this->height / 2);
    GameLevel game;
    // game.load((levelPath + "/game.lvl").c_str, this->width, this->height):

    // this->levels.push_back(menu);
    // this->levels.push_back(game);
    // this->level = 0;

    // glm::vec2 playerPos = glm::vec2(
    //     this->width / 2.0f - PLAYER_SIZE.x / 2.0f, 
    //     this->height - PLAYER_SIZE.y
    // );
    // player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    this->spawnBoids();
}

void Game::spawnBoids() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(0.0f, this->width - BOID_RADIUS * 2.0f);
    std::uniform_real_distribution<> disY(0.0f, this->height - BOID_RADIUS * 2.0f);
    std::uniform_real_distribution<> disVelX(-100.0f, 100.0f);
    std::uniform_real_distribution<> disVelY(-100.0f, 100.0f);
    
    for (int i = 0; i < 25; i++) {
        glm::vec2 randomPos = glm::vec2(disX(gen), disY(gen));
        glm::vec2 randomVel = glm::vec2(disVelX(gen), disVelY(gen));
        BoidObject boid(randomPos, BOID_RADIUS, randomVel,
                        ResourceManager::GetTexture("face"));
        this->boidList.push_back(boid);
    }
}

void Game::update(float dt)
{
    for (int i = 0; i < boidList.size(); i++)
    {
        this->boidList[i].move(dt, this->width, this->height);
    }
    // this->doCollisions();

    // if (boid->position.y >= this->height) // did boid reach bottom edge?
    // {
    //     // this->resetLevel();
    //     this->resetPlayer();
    // }
}

void Game::resetLevel()
{
    level++;
    if (this->level >= this->levels.size())
        this->level = 0;

    if (this->level == 0)
        this->levels[0].load((levelPath + "/one.lvl").c_str(), this->width, this->height / 2);
    else if (this->level == 1)
        this->levels[1].load((levelPath + "/two.lvl").c_str(), this->width, this->height / 2);
    else if (this->level == 2)
        this->levels[2].load((levelPath + "/three.lvl").c_str(), this->width, this->height / 2);
    else if (this->level == 3)
        this->levels[3].load((levelPath + "/four.lvl").c_str(), this->width, this->height / 2);
}

void Game::resetPlayer()
{
    // reset player/ball stats
    // player->size = PLAYER_SIZE;
    // player->position = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    // boid->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2.0f - BOID_RADIUS, -(BOID_RADIUS * 2.0f)), INITIAL_BOID_VELOCITY);
}

void Game::render()
{
    if(this->state == GAME_ACTIVE)
    {
        // draw background
        renderer->drawSprite(ResourceManager::GetTexture("background"), 
            glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f
        );
        // draw level
        // this->levels[this->level].draw(*renderer);
        // player->draw(*renderer);
        for (int i = 0; i < boidList.size(); i++)
        {
            this->boidList[i].draw(*renderer);
        }
    }
}

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}    

typedef std::tuple<bool, Direction, glm::vec2> Collision;   

Collision checkCollision(BoidObject &one, BoidObject &two) // AABB - Circle collision
{
    // get center point circle first 
    glm::vec2 center(one.position + one.radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabb_center(
        two.position.x + aabb_half_extents.x, 
        two.position.y + aabb_half_extents.y
    );
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    
    if (glm::length(difference) <= one.radius)
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}      

bool checkCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.position.x + one.size.x >= two.position.x &&
        two.position.x + two.size.x >= one.position.x;
    // collision y-axis?
    bool collisionY = one.position.y + one.size.y >= two.position.y &&
        two.position.y + two.size.y >= one.position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

void Game::doCollisions()
{
    // for (int i = 0; i < boidList.size(); i++;)
    // {
    //         for (int j = 0; j < boidList.size(); j++) {
    //         Collision collision = checkCollision(*boidList[i], *boidList[j]);
    //         if (std::get<0>(collision)) // if collision is true
    //         {
    //             // destroy block if not solid
    //             if (!box.isSolid)
    //                 box.destroyed = true;
    //             // collision resolution
    //             Direction dir = std::get<1>(collision);
    //             glm::vec2 diff_vector = std::get<2>(collision);
    //             if (dir == LEFT || dir == RIGHT) // horizontal collision
    //             {
    //                 boid->velocity.x = -boid->velocity.x; // reverse horizontal velocity
    //                 // relocate
    //                 float penetration = boid->radius - std::abs(diff_vector.x);
    //                 if (dir == LEFT)
    //                     boid->position.x += penetration; // move boid to right
    //                 else
    //                     boid->position.x -= penetration; // move boid to left;
    //             }
    //             else // vertical collision
    //             {
    //                 boid->velocity.y = -boid->velocity.y; // reverse vertical velocity
    //                 // relocate
    //                 float penetration = boid->radius - std::abs(diff_vector.y);
    //                 if (dir == UP)
    //                     boid->position.y -= penetration; // move boid back up
    //                 else
    //                     boid->position.y += penetration; // move boid back down
    //             }
    //         }
    //     }
    // }
    // check collisions for player pad (unless stuck)
    // Collision result = checkCollision(*boid, *player);

    // if (!boid->stuck && std::get<0>(result))
    // {
    //     // check where it hit the board, and change velocity based on where it hit the board
    //     float centerboard = player->position.x + player->size.x / 2.0f;
    //     float distance = (boid->position.x + boid->radius) - centerboard;
    //     float percentage = distance / (player->size.x / 2.0f);
    //     // then move accordingly
    //     float strength = 2.0f;
    //     glm::vec2 oldVelocity = boid->velocity;
    //     boid->velocity.x = INITIAL_BOID_VELOCITY.x * percentage * strength;
    //     //Ball->Velocity.y = -Ball->Velocity.y;
    //     boid->velocity = glm::normalize(boid->velocity) * glm::length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
    //     // fix sticky paddle
    //     boid->velocity.y = -1.0f * abs(boid->velocity.y);
    // }
}

using namespace std;

void Game::addKeyToProcess(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        this->keys[key] = true;
    else if (action == GLFW_RELEASE)
        this->keys[key] = false;
}

void Game::processInput(float dt) {
    if (!window)
        return;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (this->state != GAME_ACTIVE)
        return;

    // float velocity = PLAYER_VELOCITY * dt;
    // if (keys[GLFW_KEY_LEFT]) {
    //     if (player->position.x >= 0.0f)
    //     {
    //         player->position.x -= velocity;
    //         if (ball->stuck)
    //             ball->position.x -= velocity;
    //     }
    // }
    // if (keys[GLFW_KEY_RIGHT])
    // {
    //     if (player->position.x <= this->width - player->size.x)
    //     {
    //         player->position.x += velocity;
    //         if (ball->stuck)
    //             ball->position.x += velocity;
    //     }
    // }
    // if (this->keys[GLFW_KEY_SPACE])
    //     ball->stuck = false;
}