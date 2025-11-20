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
                        ResourceManager::GetTexture("face"), BOID_VISION_ANGLE);
        this->boidList.push_back(boid);
    }
}

void Game::update(float dt)
{
    for (int i = 0; i < boidList.size(); i++)
    {
        this->boidList[i].move(dt, this->width, this->height, boidList);
    }
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

void Game::render()
{
    if(this->state == GAME_ACTIVE)
    {
        // draw background
        renderer->drawSprite(ResourceManager::GetTexture("background"), 
            glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f
        );

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
}