#ifndef TERRAIN_MANAGER_H
#define TERRAIN_MANAGER_H

#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shaders.hpp"
#include "Camera.hpp"
#include "SpriteRenderer.hpp"
#include "FastNoiseLite.h"

class TerrainManager {
    public:
        Shader terrainShader;
        SpriteRenderer terrainRenderer;
        unsigned int terrainTexture;
        const int TERRAIN_RES = 2000;
        int WINDOW_WIDTH;
        int WINDOW_HEIGHT;
        
        TerrainManager(Camera* cam, int win_width, int win_height)
        {
            WINDOW_WIDTH = win_width;
            WINDOW_HEIGHT = win_height;

            terrainShader = Shader("../res/shaders/terrain.vert", "../res/shaders/terrain.frag");
            terrainShader.use();

            terrainShader.SetMatrix4("projection", cam->GetProjection());

            terrainRenderer = SpriteRenderer(terrainShader, NULL);


            std::vector<uint8_t> terrain = GenerateTerrain(glm::vec2(TERRAIN_RES, (int)((float)TERRAIN_RES/(float)WINDOW_WIDTH*(float)WINDOW_HEIGHT)));

            glGenTextures(1, &terrainTexture);
            glBindTexture(GL_TEXTURE_2D, terrainTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TERRAIN_RES, (int)((float)TERRAIN_RES/(float)WINDOW_WIDTH*(float)WINDOW_HEIGHT), 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, terrain.data());
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        void Render(GLFWwindow* window, Camera* cam)
        {
            terrainShader.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, terrainTexture);

            unsigned int texLoc = glGetUniformLocation(terrainShader.ID, "image");
            glUniform1i(texLoc, 0);

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            terrainShader.setVec2("mousePos", glm::vec2(xpos, ypos));
            terrainShader.SetMatrix4("projection", cam->GetProjection());
            terrainShader.setVec2("terrainSize", glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
            terrainShader.setFloat("time", glfwGetTime());

            
            terrainRenderer.DrawSprite(glm::vec2(0, 0), glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.0f, glm::vec3(1.0, 1.0, 1.0));
        }



        std::vector<uint8_t> GenerateTerrain(glm::vec2 size)
        {
            std::vector<uint8_t> terrain(size.x * size.y * 4);

            FastNoiseLite noise;
            //noise.SetSeed(100);
            noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
            noise.SetFractalType(FastNoiseLite::FractalType_FBm);
            noise.SetFractalOctaves(5);

            for (int x = 0; x < size.x; x++)
            {
                for (int y = 0; y < size.y; y++)
                {
                    int id = 4 * (y * size.x + x);

                    double noiseVal = noise.GetNoise((float) x/3, (float) y/3)/2+0.5;

                    float maxD = glm::min(size.x, size.y);
                    maxD = (maxD/2) * (maxD/2);

                    float disFronCenter = glm::distance(glm::vec2(x, y), glm::vec2(size.x/2, size.y/2));

                    noiseVal *= 1-(disFronCenter*disFronCenter / maxD);

                    noiseVal *= 4.;
                    
                    terrain[id] = (int)(glm::clamp(noiseVal-3., 0.0, 1.0)*255.0);
                    terrain[id + 1] = (int)(glm::clamp(noiseVal-2., 0.0, 1.0)*255.0);
                    terrain[id + 2] = (int)(glm::clamp(noiseVal-1., 0.0, 1.0)*255.0);
                    terrain[id + 3] = (int)(glm::clamp(noiseVal, 0.0, 1.0)*255.0);
                }
            }



            return terrain;
        }
};


#endif