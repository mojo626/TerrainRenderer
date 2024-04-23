#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

class Inputs {
    public:
        bool LEFT = false;
        bool RIGHT = false;
        bool UP = false;
        bool DOWN = false;
};

class WindowManager {
    public:
        WindowManager(){}

        GLFWwindow* init(int WINDOW_WIDTH, int WINDOW_HEIGHT) {
            //Set glfw settings
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            #ifdef __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            #endif
            glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);



            //create window instance
            GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terrain Renderer", NULL, NULL);
            if (window == NULL)
            {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return NULL;
            }
            glfwMakeContextCurrent(window);


            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 



            //Initialize glad for openGL
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cout << "Failed to initialize GLAD" << std::endl;
                return NULL;
            }    

            // // Setup Dear ImGui context
            // IMGUI_CHECKVERSION();
            // ImGui::CreateContext();
            // ImGuiIO& io = ImGui::GetIO();

            // ImGui::StyleColorsDark();

            // ImGui_ImplGlfw_InitForOpenGL(window, true);
            // ImGui_ImplOpenGL3_Init("#version 330");

            //tell openGL size of viewport
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


            return window;
        }

        //process input for window
        Inputs processInput(GLFWwindow *window)
        {
            Inputs inputs;

            if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
                
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                inputs.UP = true;
            
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                inputs.DOWN = true;

            if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                inputs.LEFT = true;

            if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                inputs.RIGHT = true;


            return inputs;
        }

    private:
        //function that is called when window is resized
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
};


#endif