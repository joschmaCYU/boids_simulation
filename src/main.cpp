#include "window/window.h"
#include "game_loop.h"
#include "game.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

int main() {
    GLFWwindow* window; // Declare the 'window' variable

    initGLFW();
    window = createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World"); // Assign a value to the 'window' variable

    Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
    game.init();
    game.setWindow(window);

    // Set the window user pointer to the game instance so callbacks can access it
    glfwSetWindowUserPointer(window, &game);

    game_loop(window, &game);

    return 0;
}