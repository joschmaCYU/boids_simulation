#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstring>

#include <glm/glm.hpp>

#include "../graphics/shader_class.h"
#include "resource_manager.h"
#include "../graphics/texture.h"
#include "../graphics/sprite_renderer.h"

#include "game_loop.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

// Screen dimensions
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Game state
bool Keys[1024];


void game_loop(GLFWwindow *window, Game* game)
{
    // Load shaders using ResourceManager
    stbi_set_flip_vertically_on_load(true);

    // Delta time variables
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // OpenGL configuration
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main game loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        // Process input
        game->processInput(deltaTime);

        // Update game state
        game->update(deltaTime);

        // Clear and render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game->render();

        glfwSwapBuffers(window);
    }

    // Delete all resources as loaded using the resource manager
    ResourceManager::Clear();
}