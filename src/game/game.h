#ifndef GAME_H
#define GAME_H

#include <vector>
#include "game_level.h"
#include <GLFW/glfw3.h>
#include "../game/object/boid.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
}; 

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

// Defines a Collision typedef that represents collision data
typedef std::tuple<bool, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

// Initial velocity of the boid
const glm::vec2 INITIAL_BOID_VELOCITY(100.0f, -350.0f);
// Radius of the boid object
const float BOID_RADIUS = 12.5f;
const float BOID_VISION_ANGLE = 120.0f;

class Game
{
    public:
        // game state
        GameState    state;
        bool         keys[1024];
        unsigned int width, height;
        GLFWwindow*  window;
        std::vector<BoidObject> boidList;
        // constructor/destructor
        Game(unsigned int width, unsigned int height);
        ~Game();
        // initialize game state (load all shaders/textures/levels)
        void init();
        // set the window for direct key queries
        void setWindow(GLFWwindow* w) { window = w; }
        // game loop
        void spawnBoids();
        void addKeyToProcess(GLFWwindow* window, int key, int scancode, int action, int mods);
        void processInput(float dt);
        void update(float dt);
        void render();
        std::vector<GameLevel> levels;
        unsigned int level;
        // reset
        void resetLevel();
};



#endif