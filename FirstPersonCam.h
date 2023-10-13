//
// Created by forag on 10/13/2023.
//

#ifndef MP_FIRSTPERSONCAM_H
#define MP_FIRSTPERSONCAM_H


#include <CSCI441/Camera.hpp>

/**
 * @brief A camera that implements a ArcballCam camera model.
 */
class FirstPersonCam final : public CSCI441::Camera {
public:
    /**
     * creates a ArcballCam object with the specified perspective projection
     * @param aspectRatio aspect ratio of view plane (defaults to 1.0f)
     * @param fovy vertical field of view (defaults to 45.0f)
     * @param nearClipPlane near z clip plane (defaults to 0.001f)
     * @param farClipPlane far z clip plane (defaults to 1000.0f)
     * @note field of view specified in degrees
     */
    explicit FirstPersonCam(GLfloat aspectRatio = 1.0f, GLfloat fovy = 45.0f, GLfloat nearClipPlane = 1.0f, GLfloat farClipPlane = 100.0f);

    /**
     * @brief converts spherical theta & phi to cartesian x,y,z direction vector
     * @note sets the camera's direction vector to point outward from a sphere centered
     * at the camera's position and updates the camera's look at point to be a point on
     * the sphere offset from the camera's position.
     * @note internally sets the camera's view matrix
     */
    void recomputeOrientation() final;

    /**
     * @brief updates the camera's position by the adding the camera's direction to the camera's position
     * @param movementFactor  distance factor to scale the movement step
     * @note internally sets the camera's view matrix
     */
    void moveForward(GLfloat movementFactor) final;

    /**
     * @brief updates the camera's position by the adding the camera's negative direction to the camera's position
     * @param movementFactor distance factor to scale the movement step
     * @note internally sets the camera's view matrix
     */
    void moveBackward(GLfloat movementFactor) final;


private:
    // updates the look at point and recalculates the view matrix
    void _updateFreeCam2CameraViewMatrix();

    GLfloat _fovy;
    GLfloat _aspectRatio;
    GLfloat _nearClipPlane;
    GLfloat _farClipPlane;

};
#endif //MP_FIRSTPERSONCAM_H
