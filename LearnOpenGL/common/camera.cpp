//
//  camera.cpp
//  transformations
//
//  Created by william on 2020/4/12.
//  Copyright © 2020 william. All rights reserved.
//

#include "camera.hpp"


Camera::Camera(glm::vec3 position, glm::vec3 up,float yaw, float pitch)
:mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
mMovementSpeed(SPEED),
mMouseSensitivity(SENSITIVEITY),
mZoom(ZOOM)
{
    mPosition = position;
    mWorldUp  = up;
    mYaw      = yaw;
    mPitch    = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
:mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
mMovementSpeed(SPEED),
mMouseSensitivity(SENSITIVEITY)
{
    mPosition = glm::vec3(posX, posY, posZ);
    mWorldUp  = glm::vec3(upX, upY, upZ);
    mYaw      = yaw;
    mPitch    = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

glm::vec3 Camera::getCameraPosition()
{
    return mPosition;
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = mMovementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        mPosition += mFront * velocity;
    if (direction == Camera_Movement::BACKWARD)
        mPosition -= mFront * velocity;
    if (direction == Camera_Movement::LEFT)
        mPosition -= mRight * velocity;
    if (direction == Camera_Movement::RIGHT)
        mPosition += mRight * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mMouseSensitivity;
    yoffset *= mMouseSensitivity;
    
    mYaw   += xoffset;
    mPitch += yoffset;
    if (constrainPitch)
        mPitch = glm::clamp(mPitch, -89.0f, 89.0f);

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    mZoom -= yoffset;
    mZoom = glm::clamp(mZoom, 1.0f, 60.0f);
}

void Camera::updateCameraVectors()
{
    // Caculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp = glm::normalize(glm::cross(mRight, mFront));
}
