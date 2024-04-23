#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "util/WindowManager.hpp"
#include "util/Shaders.hpp"
#include "util/SpriteRenderer.hpp"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    std::cout << "Hello World!";

    WindowManager man = WindowManager();

    GLFWwindow* window = man.init(WINDOW_WIDTH, WINDOW_HEIGHT);

    Shader terrainShader("../res/shaders/terrain.vert", "../res/shaders/terrain.frag");
    terrainShader.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 
        static_cast<float>(WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);

    terrainShader.SetMatrix4("projection", projection);

    SpriteRenderer terrainRenderer = SpriteRenderer(terrainShader, "../res/images/Flower.png");


    while (!glfwWindowShouldClose(window))
    {
       // update delta deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwSetWindowTitle(window, ("Terrain Renderer | FPS " + std::to_string(1000/deltaTime)).c_str());

        //rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        terrainShader.use();
        terrainRenderer.DrawSprite(glm::vec2(0, 0), glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.0f, glm::vec3(1.0, 1.0, 1.0));


        //check events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();  
    }

    glfwTerminate();
    return 0;
}