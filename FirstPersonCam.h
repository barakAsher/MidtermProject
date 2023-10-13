//
// Created by Porter Fredrickson on 10/12/23.
//

#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class FirstPersonCamera {
private:
    glm::vec3 position;  // Camera position
    glm::vec3 front;     // Camera front direction
    glm::vec3 up;        // Camera up direction
    glm::vec3 right;     // Camera right direction
    glm::vec3 worldUp;   // World up direction

    float yaw;           // Yaw angle
    float pitch;         // Pitch angle

    glm::mat4 projectionMatrix;  // Add this line to hold the projection matrix
    //glm::vec3 position;

    float movementSpeed;
    float mouseSensitivity;

    // Update the camera vectors based on yaw and pitch
    void updateCameraVectors();

public:
//    FirstPersonCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    FirstPersonCamera(glm::vec3 position);
    // Get the view matrix
    glm::mat4 getViewMatrix();

    // Process input to update the camera's position and orientation
    void processKeyboard(int direction, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void processMouseScroll(float yOffset);

    // Getters and setters for camera parameters
    float getMovementSpeed() const;
    float getMouseSensitivity() const;
    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;

    glm::mat4 getProjectionMatrix() const;

    void setPosition(glm::vec3 newPosition);
};

#endif // FIRST_PERSON_CAMERA_H
