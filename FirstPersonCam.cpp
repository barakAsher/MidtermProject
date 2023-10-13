//
// Created by Porter Fedrickson on 10/12/23.
//

#include "FirstPersonCam.h"

#include <iostream>

//FirstPersonCamera::FirstPersonCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
//        : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(2.5f), mouseSensitivity(0.1f), yaw(yaw), pitch(pitch)
//
//{
//    this->position = position;
//    this->worldUp = up;
//    updateCameraVectors();
//}

FirstPersonCamera::FirstPersonCamera(glm::vec3 initialPosition)
        : position(initialPosition), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(-90.0f), pitch(0.0f)
{
    updateCameraVectors();
    position = initialPosition;
    float fov = 45.0f;       // Field of View
    float aspectRatio = 16.0f / 9.0f;  // Adjust as needed
    float nearPlane = 0.1f;  // Near clipping plane
    float farPlane = 100.0f; // Far clipping plane

    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::mat4 FirstPersonCamera::getProjectionMatrix() const {
    return projectionMatrix;
}

void FirstPersonCamera::setPosition(glm::vec3 newPosition) {
    position = newPosition;
}


void FirstPersonCamera::updateCameraVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Recalculate right and up vectors
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::mat4 FirstPersonCamera::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void FirstPersonCamera::processKeyboard(int direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == 0) // Move forward
        position += front * velocity;
    if (direction == 1) // Move backward
        position -= front * velocity;
    if (direction == 2) // Move right
        position += right * velocity;
    if (direction == 3) // Move left
        position -= right * velocity;
}

void FirstPersonCamera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Constrain pitch to avoid flipping when looking up or down
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void FirstPersonCamera::processMouseScroll(float yOffset)
{
    // Not implementing zoom for a basic first-person camera
    // You can add zoom functionality based on your requirements
    std::cout << "Scroll input: " << yOffset << std::endl;
}

float FirstPersonCamera::getMovementSpeed() const
{
    return movementSpeed;
}

float FirstPersonCamera::getMouseSensitivity() const
{
    return mouseSensitivity;
}

glm::vec3 FirstPersonCamera::getPosition() const
{
    return position;
}


glm::vec3 FirstPersonCamera::getFront() const
{
    return front;
}
