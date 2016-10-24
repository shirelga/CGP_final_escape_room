//
// Created by Shirel Gazit on 6/2/16.
//

#include <GL/glew.h>
#include <GLUT/glut.h>
#include "Camera.h"

#define RIGHT true
#define LEFT false
#define FRONT true
#define BACK false
#define MOVE 0.5f

Camera* Camera::singletone = nullptr;

Camera::Camera()
{
//    front
    _pos = glm::vec3(0.f, 20.f, -50.f);
    _dir = glm::vec3(0.0f, 0.0f, 1.0f);
    _up = glm::vec3(0.f, 1.f, 0.f);

//    // back
//    _pos = glm::vec3(0.f, 0.f, 5.f);
//    _dir = glm::vec3(0.0f, 0.0f, -1.0f);
//    _up = glm::vec3(0.f, 1.f, 0.f);

//    //up
//    _pos = glm::vec3(0.f, 10.f, 0.f);
//    _dir = glm::vec3(0.0f, -1.0f, 0.0f);
//    _up = glm::vec3(1.f, 0.f, 0.f);

//    //down
//    _pos = glm::vec3(0.f, -5.f, 0.f);
//    _dir = glm::vec3(0.0f, 1.0f, 0.0f);
//    _up = glm::vec3(-1.f, 0.f, 0.f);



    _fov = 45.f;
    _View = glm::lookAt(_pos, (_pos + _dir), _up);
    float ratio = (float)_screen_width/(float)_screen_hieght;
//    _Perspective = glm::perspectiveFov(_fov, static_cast<float
//            >(glutGet(GLUT_SCREEN_WIDTH)), static_cast<float>(glutGet(GLUT_SCREEN_HEIGHT)), 0.1f, 300.f);
    _Perspective = glm::perspective(_fov, 1.f, 0.1f, 300.f);
};


Camera& Camera::getInstance()
{
    if(singletone == nullptr)
    {
        singletone = new Camera();
    }

    return *singletone;
}


void Camera::destroy() {
    delete singletone;
}

void Camera::keyPressed(int key, int x, int y) {
    switch (key)
    {
        case 'q':
            straf(LEFT);
            break;
        case 'w':
            walk(FRONT);
            break;
        case 'e':
            straf(RIGHT);
            break;
        case 'a':
            rotate(LEFT);
        case 's':
            walk(BACK);
            break;
        case 'd':
            rotate(RIGHT);
            break;
        default:
            std::cerr<< static_cast<char>(key)<<" not initialize key"<<std::endl;
            break;
    }
}

void Camera::straf(bool side) {
    int axis = get_max_axis(_dir);
    switch (side)
    {
        case RIGHT:
            if(axis)
            {
                if(_dir[axis] < 0)
                {
                    _pos.x += MOVE;
                }
                else
                {
                    _pos.x -= MOVE;
                }
            }
            else
            {
                if(_dir[axis] < 0)
                {
                    _pos.z -= MOVE;
                }
                else
                {
                    _pos.z += MOVE;
                }
            }
            break;
        case LEFT:
            if(axis)
            {
                if(_dir[axis] < 0)
                {
                    _pos.x -= MOVE;
                }
                else
                {
                    _pos.x += MOVE;
                }
            }
            else
            {
                if(_dir[axis] < 0)
                {
                    _pos.z += MOVE;
                }
                else
                {
                    _pos.z -= MOVE;
                }
            }
            break;
    }
    update_view();
}

void Camera::rotate(bool side) {
    switch (side)
    {
        case LEFT:

            _dir = glm::rotateY(_dir, glm::radians(90.f));
            break;
        case RIGHT:
            _dir = glm::rotateY(_dir, glm::radians(-90.f));
            break;
    }
    update_view();
}

void Camera::walk(bool front_back) {
    int axis = get_max_axis(_dir);
    switch (front_back)
    {
        case FRONT:

            if(axis)
            {
                if(_dir[axis] < 0)
                {
                    _pos.z -= MOVE;
                }
                else
                {
                    _pos.z += MOVE;
                }
            }
            else
            {
                if(_dir[axis] < 0)
                {
                    _pos.x -= MOVE;
                }
                else
                {
                    _pos.x += MOVE;
                }
            }
            break;
        case BACK:
            if(axis)
            {
                if(_dir[axis] < 0)
                {
                    _pos.z += MOVE;
                }
                else
                {
                    _pos.z -= MOVE;
                }
            }
            else
            {
                if(_dir[axis] < 0)
                {
                    _pos.x += MOVE;
                }
                else
                {
                    _pos.x -= MOVE;
                }
            }
            break;
    }
    update_view();
}

int Camera::get_max_axis(const glm::vec3 &v) {
    if(glm::abs(v.x) >= glm::abs(v.z))
    {
        return 0;
    }
    return 2;
}

void Camera::update_view() {
    _View = glm::lookAt(_pos, (_pos + _dir), _up);
}