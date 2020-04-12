//
//  camera.hpp
//  transformations
//
//  Created by william on 2020/4/12.
//  Copyright © 2020 william. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp
#include "base.h"

enum class Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// default camera values
const float YAW          = -90.0f;
const float PITCH        = 0.0f;
const float SPEED        = 2.5f;
const float SENSITIVEITY = 0.1f;
const float ZOOM         = 45.0f;


class Camera
{
public:
    Camera() = default;
    ~Camera() = default;
    Camera(const Camera&) = default;
    Camera(Camera&&) = default;
    Camera& operator=(const Camera&) = default;
    Camera& operator=(Camera&&) = default;
public:
    // Camera Attributes
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;
    // Euler Angles
    float mYaw;
    float mPitch;
    // Camera options
    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    glm::mat4 getViewMatrix();
    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void processMouseScroll(float yoffset);
private:
    void updateCameraVectors();
};

#endif /* camera_hpp */
