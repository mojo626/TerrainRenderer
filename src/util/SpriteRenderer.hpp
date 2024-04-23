#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Shaders.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "texture.h"
#include "stb_image.h"

class SpriteRenderer {
    public:
        const char *filepath;
        Texture2D texture;
        Shader shader;
        SpriteRenderer(Shader &shader, const char *file) {
            this->shader = shader;

            filepath = file;
            if (filepath != NULL)
            {
                texture = loadTextureFromFile(filepath, true);
                this->shader.setBool("isTexture", true);
            } else {
                this->shader.setBool("isTexture", false);
            }

            this->initRendererData();
        }


        /// @brief Draws a sprite to the screen
        /// @param position the top left position of the sprite
        /// @param size the size of the sprite
        /// @param rotate rotation of the sprite (in degrees)
        /// @param color the color of the sprite (0-1)
        void DrawSprite(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
            //TODO: Add textures
            this->shader.use();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(position, 0.0f)); //first, translate position

            model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
            model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
            model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

            model = glm::scale(model, glm::vec3(size, 1.0f));

            
            if (filepath != NULL)
            {
                glActiveTexture(GL_TEXTURE0);
                texture.Bind();
            }
            



            
            this->shader.SetMatrix4("model", model);

            this->shader.setVec3("spriteColor", color);

            this->shader.setVec3("offset", glm::vec3(position, 0.0));


            // std::vector<glm::vec3> lights;
            // for (int i = 0; i < 100; i++)
            // {
            //     lights.push_back(glm::vec3(0.0, 0.0, 0.0));
            // }
            
            // lights[0] = glm::vec3(10, 0, 0);
            // lights[1] = glm::vec3(1, 1, 0);
            // this->shader.setVec3Arr("lightPos", lights);
            // this->shader.setInt("numLights", lights.size());
            

            glBindVertexArray(this->quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

        }

        Texture2D loadTextureFromFile(const char *file, bool alpha) {
            // create texture object
            Texture2D texture;
            if (alpha) {
            texture.Internal_Format = GL_RGBA;
            texture.Image_Format = GL_RGBA;
            }
            // load image
            int width, height, nrChannels;
            unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
            // now generate texture
            texture.Generate(width, height, data);
            // and finally free image data
            stbi_image_free(data);
            std::cout << "Texture loaded" << std::endl;
            std::cout << texture.Width << std::endl;
            return texture;
        }

        void SetTile(glm::vec2 coords, int numTiles)
        {
            this->shader.setVec3("tileData", glm::vec3(coords, numTiles));
        }

    private:
        unsigned int quadVAO;

        void initRendererData() {
            // configure VAO/VBO
            unsigned int VBO;
            float vertices[] = { 
                // pos      // tex
                0.0f, 1.001f, 0.0f, 1.0f,
                1.001f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 

                0.0f, 1.001f, 0.0f, 1.0f,
                1.001f, 1.001f, 1.0f, 1.0f,
                1.001f, 0.0f, 1.0f, 0.0f
            };

            glGenVertexArrays(1, &this->quadVAO);
            glGenBuffers(1, &VBO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(this->quadVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
};


#endif