//
// Created by Porter Fedrickson on 10/12/23.
//

#include "FirstPersonCam.h"

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

void FirstPersonCamera::setYaw(float newYaw)
{
    yaw = newYaw;
    updateCameraVectors();
}

float FirstPersonCamera::getYaw() const
{
    return yaw;
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