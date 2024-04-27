#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shaders.hpp"
#include "Camera.hpp"
#include "SpriteRenderer.hpp"
#include "FastNoiseLite.h"


class Player {
    public:
        Shader shader;
        SpriteRenderer renderer;
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;

        Player(Camera* cam, int screenWidth, int screenHeight)
        {
            SCREEN_HEIGHT = screenHeight;
            SCREEN_WIDTH = screenWidth;

            shader = Shader("../res/shaders/player.vert", "../res/shaders/player.frag");
            shader.use();

            shader.SetMatrix4("projection", cam->GetProjection());

            renderer = SpriteRenderer(shader, "../res/images/Boat.png");
        }

        void Render(Camera* cam, Inputs input)
        {
            float speed = 1;
            glm::vec2 move;
            if (input.DOWN)
            {
                move.y -= speed;
            } 
            if (input.LEFT)
            {
                move.x -= speed;
            } 
            if (input.RIGHT)
            {
                move.x += speed;
            }
            if (input.UP)
            {
                move.y += speed;
            }

            float rot = atan2(move.x, move.y);
            

            shader.use();

            shader.SetMatrix4("projection", cam->GetProjection());

            renderer.DrawSprite(glm::vec2(cam->pos.x + SCREEN_WIDTH/4-10, cam->pos.y + SCREEN_HEIGHT/4-10), glm::vec2(20, 20), rot/3.14*180 , glm::vec3(1.0, 1.0, 1.0));
        }
};


#endif