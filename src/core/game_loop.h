#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../game/game.h"

// Main game loop function
void game_loop(GLFWwindow* window, Game* game);

#endif // GAME_LOOP_H