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


    float yaw; // Yaw angle
    float pitch;         // Pitch angle

    glm::mat4 projectionMatrix;  // Add this line to hold the projection matrix

    // update camera when it's rotated with A nd D keys
    void updateCameraVectors();

public:
//    FirstPersonCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    FirstPersonCamera(glm::vec3 position);
    // Get the view matrix
    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix() const;

    void setPosition(glm::vec3 newPosition);


    void setYaw(float newYaw);

    float getYaw() const;

    void updateYawFromPlayer(float playerYaw);
};

#endif // FIRST_PERSON_CAMERA_H
