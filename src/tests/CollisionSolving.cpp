#include "CollisionSolving.h"

#ifndef REPLIT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <iostream>
#include <stdexcept>
#include <chrono>

#include "../include/Screen.h"
#include "../include/Block.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// For all the blocks
#include <vector>
std::vector<Block*> blocks;

int main() {

#ifndef REPLIT

    const int INITIAL_WINDOW_WIDTH = 1920;
    const int INITIAL_WINDOW_HEIGHT = 1080;

    if (!glfwInit()) throw std::runtime_error("glfw init failed");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 5); // Anti-aliasing!

    auto* window = glfwCreateWindow(
            INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Physiks Simulator",
            nullptr, nullptr
    );
    if (!window) throw std::runtime_error("window creation failed");

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) throw std::runtime_error("glew init failed");

    Block::init_static_render_cache(); // call once when app starts

#endif

    // Example of rendering a yellow square
    // Anyone can delete this if you want to
    Block b(100, 900, 100, 100, 1);
    b.set_rotation(M_PI/4);
    blocks.push_back(&b);
    Block b2(100, 300, 100, 100, 1);
    blocks.push_back(&b2);

    // Ground
    Block b3(500, 500, 2000, 10, 0);
    b3.is_static = true;
    blocks.push_back(&b3);

#ifndef REPLIT
    for (auto* block : blocks) block->update_render_cache();
#endif

    b.set_color(Color::WHITE); // or Color(255, 255, 0)


#ifndef REPLIT

    Screen screen;
    screen.blocks = blocks;

    double interval_between_frames = 1.0 / fps;
    auto startTime = std::chrono::high_resolution_clock::now(), currTime = startTime, prevTime = startTime;

    int frameCounter = 0;

    bool done = false;

    while (!glfwWindowShouldClose(window)) {
        glUseProgram(Block::get_gl_program());
        currTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = currTime - prevTime;
        double durationInSeconds = static_cast<double>(duration.count());


        if (durationInSeconds > interval_between_frames) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            
            screen.update();
            prevTime = currTime;
            glfwSwapBuffers(window);

            if (!done) b2.apply_accel(Vect2(0, 9.81));

            if (!done && b.position.y - sqrt(2) * 50 < 350) {
                done = true;
                auto cg = CollisionGroup(&b, &b2);
                cg.solve();
            }

        }
        glfwPollEvents();
    }

#endif

}