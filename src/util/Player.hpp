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

        glm::vec2 move;
        double curRot;

        glm::vec2 pos;
        double speed;
        double waterForce = 0.9;

        Player(Camera* cam, int screenWidth, int screenHeight)
        {
            SCREEN_HEIGHT = screenHeight;
            SCREEN_WIDTH = screenWidth;

            shader = Shader("../res/shaders/player.vert", "../res/shaders/player.frag");
            shader.use();

            shader.SetMatrix4("projection", cam->GetProjection());

            renderer = SpriteRenderer(shader, "../res/images/Boat.png");
        }

        void ProcessInputs(Inputs input)
        {
            
        }

        void Render(Camera* cam, Inputs input)
        {
            float speed = 1;

            if (input.DOWN)
            {
                move.y = -speed;
                if(!input.LEFT && !input.RIGHT)
                {
                    move.x = 0;
                }
            } 
            if (input.LEFT)
            {
                move.x = -speed;
                if(!input.UP && !input.DOWN)
                {
                    move.y = 0;
                }
            } 
            if (input.RIGHT)
            {
                move.x = speed;
                if(!input.UP && !input.DOWN)
                {
                    move.y = 0;
                }
            }
            if (input.UP)
            {
                move.y = speed;
                if(!input.LEFT && !input.RIGHT)
                {
                    move.x = 0;
                }
            }

            float rot = atan2(move.x, move.y);

            if (rot < curRot) {
                curRot -= 0.1;
            }
            if (rot > curRot) {
                curRot += 0.1;
            }
            

            shader.use();

            shader.SetMatrix4("projection", cam->GetProjection());

            renderer.DrawSprite(glm::vec2(cam->pos.x + SCREEN_WIDTH/4-10, cam->pos.y + SCREEN_HEIGHT/4-10), glm::vec2(20, 20), curRot/3.14*180 , glm::vec3(1.0, 1.0, 1.0));
        }
};


#endif