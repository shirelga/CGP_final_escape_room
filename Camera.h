//
// Created by Shirel Gazit on 6/2/16.
//

#ifndef KESSEL_RUN_1_CAMERA_H
#define KESSEL_RUN_1_CAMERA_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr

class Camera {
public:
    static Camera& getInstance();

    inline glm::mat4 getView()
    {
        return _View;
    }

    inline glm::mat4 getPerspective()
    {
        return _Perspective;
    }

    inline glm::vec3 getPos()
    {
        return _pos;
    }

    void static destroy();

    //Set all the view changes for key moves
//    void setViewRotate(int leftRight);
//    void setViewTranslate(int backFroward);
//    void updateCameraTranslate(float vel);
//    void updateCameraRotate(float x, float y, float angle);
//    void updateCameraRotate(float angle, float y_rot);
//    float hDistFromPlayer();
//    float vDistFromPlayer();
//    void getDistFromCam();
//    void getPitch();


private:
    glm::vec3 _dir;
    glm::vec3 _pos;
    glm::vec3 _up;
    glm::mat4 _Perspective;
    glm::mat4 _View;
    float _playerDistFromCam;
    float _playerPitch;
    float _fov;
    glm::vec3 _playerOffset;
    static Camera* singletone;
    Camera();

    Camera(Camera const&) = delete;
    void operator=(Camera const&) = delete;
};


#endif //KESSEL_RUN_1_CAMERA_H