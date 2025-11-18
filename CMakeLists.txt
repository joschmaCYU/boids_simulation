cmake_minimum_required(VERSION 3.5...3.27)
project(game_engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)

set(SOURCE_FILES 
    src/main.cpp
    src/window/window.cpp
    src/core/game_loop.cpp
    src/glad.c
    src/graphics/shader_class.cpp
    src/graphics/texture.cpp
    src/core/resource_manager.cpp
    src/game/game.cpp
    src/game/game_level.cpp
    src/game/game_object.cpp
    src/game/object/boid.cpp
    src/graphics/sprite_renderer.cpp)

add_executable(game_engine ${SOURCE_FILES})

target_include_directories(game_engine PUBLIC 
    "src/include"
    "src/core"
    "src/graphics" 
    "src/game"
    "src/platform")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(game_engine glfw OpenGL::GL)