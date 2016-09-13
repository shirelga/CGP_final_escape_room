//
// Created by Shirel Gazit on 6/2/16.
//

#include "Camera.h"
Camera* Camera::singletone = nullptr;

Camera::Camera()
{
//    front
    _pos = glm::vec3(0.f, 0.f, -5.f);
    _dir = glm::vec3(0.0f, 0.0f, 1.0f);
    _up = glm::vec3(0.f, 1.f, 0.f);

//    // back
//    _pos = glm::vec3(0.f, 0.f, 5.f);
//    _dir = glm::vec3(0.0f, 0.0f, -1.0f);
//    _up = glm::vec3(0.f, 1.f, 0.f);

//    //up
    _pos = glm::vec3(0.f, 10.f, 0.f);
    _dir = glm::vec3(0.0f, -1.0f, 0.0f);
    _up = glm::vec3(1.f, 0.f, 0.f);

//    //down
//    _pos = glm::vec3(0.f, -5.f, 0.f);
//    _dir = glm::vec3(0.0f, 1.0f, 0.0f);
//    _up = glm::vec3(1.f, 0.f, 0.f);



    _fov = 45.f;
    _View = glm::lookAt(_pos, _pos + _dir, _up);
    _Perspective = glm::perspective(_fov, 1.f, 0.1f, 100.f);
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

//void Camera::setViewRotate(int leftRight)
//{
//    if(leftRight)
//    {
//        _dir = glm::vec3(glm::rotate(glm::mat4(1.f),glm::radians(-1.f), glm::vec3(0.f, 1.f, 0.f)) *  glm::vec4(_dir,1.f));
////            std::cout<<_dir.x<<" "<<_dir.y<<" "<<_dir.z<<std::endl;
////            _View = glm::rotateY(_View, glm::radians(5.f), glm::vec3(0.f, 1.f, 0.f));
//    }
//    else
//    {
//        _dir = glm::vec3(glm::rotate(glm::mat4(1.f), glm::radians(1.f), glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(_dir,1.f));
//    }
//#ifdef DBG
//    print();
//#endif
//    _View = glm::lookAt(_pos, _pos + _dir, _up);
//
//}

//void Camera::setViewTranslate(int backFroward)
//{
//    glm::vec3 move = glm::vec3(0.1f * _dir.x, 0.f, 0.1f * _dir.z);
//    if(backFroward)
//    {
////        if(fabsf(_pos.x + move.x) > 0.9999999f || fabsf(_pos.z + move.z) > 0.9999999f)
////        { return;}
//        _tempPos = _pos;
//        _pos = glm::vec3(glm::translate(glm::mat4(1.f), move) * glm::vec4(_pos,1.f));
//    }
//    else
//    {
////        if(fabsf(_pos.x - move.x) > 0.9999999f || fabsf(_pos.z - move.z) > 0.9999999f)
////        { return;}
//        _tempPos = _pos;
//        _pos = glm::vec3(glm::translate(glm::mat4(1.0f), -1.f * move) * glm::vec4(_pos,1.f));
//    }
//#ifdef DBG
//    print();
//#endif
//    _View = glm::lookAt(_pos, _pos + _dir, _up);
//}

//void Camera::updateCameraTranslate(float vel) {
//    _pos  = glm::vec3(_player->get_model() * glm::vec4(_playerOffset, 1));
//    _dir = _player->get_dir();
//    _View = glm::lookAt(_pos, _pos + _dir, _up);
//}
//
////void Camera::updateCameraRotate(float x, float z, float angle) {
////    _pos.x += x;
////    _pos.z += z;
////    _dir = glm::vec3(glm::rotateY(glm::mat4(1.f), angle, glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(_dir,1.f));
////    _View = glm::lookAt(_pos, _pos + _dir, _up);
//
//void Camera::updateCameraRotate(float angle, float y_rot) {
//    _pos = glm::vec3(_player->get_model() * glm::vec4(_playerOffset, 1));
//    _dir = _player->get_dir();
//    _up = _player->get_up();
//    _View = glm::lookAt(_pos, _pos + _dir, _up);
//}
//
//void Camera::getDistFromCam() {
//    _playerDistFromCam =  glm::distance(_pos, _player->get_center());
//}
//
//void Camera::getPitch() {
//    glm::vec3 center = _player->get_center();
//    float hDist = center.z - _pos.z;
////    float d = getDistFromCam();
//    _playerPitch = glm::acos(hDist / _playerDistFromCam);
//}
//
//float Camera::hDistFromPlayer() {
//    return _playerDistFromCam * glm::cos(_playerPitch);
//}
//
//float Camera::vDistFromPlayer() {
//    return _playerDistFromCam * glm::sin(_playerPitch);
//}