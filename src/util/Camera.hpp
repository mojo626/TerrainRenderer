#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "WindowManager.hpp"

class Camera {
    public:
        glm::vec2 pos;
        float scale = 0.5f;
        float camSpeed = 2.0f;
        glm::vec2 screenSize;

        Camera(glm::vec2 pos, float scale, glm::vec2 screenSize) {
            this->pos = pos;
            this->scale = scale;
            this->screenSize = screenSize;
        }
        glm::mat4 GetProjection()
        {
            return glm::ortho(pos.x, scale * screenSize.x + pos.x, 
            scale * screenSize.y + pos.y, pos.y, -1.0f, 1.0f);
        }

        void Move(Inputs move)
        {
            float speed = scale * camSpeed;
            if (move.DOWN)
            {
                pos.y += speed;
            } 
            if (move.LEFT)
            {
                pos.x -= speed;
            } 
            if (move.RIGHT)
            {
                pos.x += speed;
            }
            if (move.UP)
            {
                pos.y -= speed;
            }
        }

    private:
};

#endif