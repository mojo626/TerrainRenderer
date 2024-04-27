#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "util/WindowManager.hpp"
#include "util/Shaders.hpp"
#include "util/SpriteRenderer.hpp"
#include "util/FastNoiseLite.h"
#include "util/Camera.hpp"
#include "util/TerrainManager.hpp"
#include "util/Player.hpp"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
float deltaTime = 0.0f;
float lastFrame = 0.0f;



int main() {

    WindowManager man = WindowManager();

    GLFWwindow* window = man.init(WINDOW_WIDTH, WINDOW_HEIGHT);

    
    Camera cam = Camera(glm::vec2(0, 0), 0.5, glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));

   

    TerrainManager terrainMan = TerrainManager(&cam, WINDOW_WIDTH, WINDOW_HEIGHT);
    Player player = Player(&cam, WINDOW_WIDTH, WINDOW_HEIGHT);

    


    while (!glfwWindowShouldClose(window))
    {
       // update delta deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cam.Move(man.processInput(window));

        glfwSetWindowTitle(window, ("Terrain Renderer | FPS " + std::to_string(1/deltaTime)).c_str());

        //rendering
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        terrainMan.Render(window, &cam);
        player.Render(&cam, man.processInput(window));
        //CONSOLODATE INTO ONE PROCESSINPUT

        //check events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();  
    }

    glfwTerminate();
    return 0;
}

