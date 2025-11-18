#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <vector>
#include "sprite_renderer.h"
#include "game_object.h"

class GameLevel
{
public:
    // level state
    std::vector<GameObject> bricks;
    // constructor
    GameLevel() { }
    // loads level from file
    void load(const char *file, unsigned int levelWidth, unsigned int levelHeight);
    // render level
    void draw(SpriteRenderer &renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool isCompleted();
private:
    // initialize level from tile data
    void init(std::vector<std::vector<unsigned int>> tileData, 
              unsigned int levelWidth, unsigned int levelHeight);
};

#endif