/**
 * @file ArcballCam.hpp
 * @brief Arcball camera implementation
 * @author Eddie Voss
 *
 *	These functions, classes, and constants help minimize common
 *	code that needs to be written.
 */

#ifndef CSCI441_ARCBALL_CAM_HPP
#define CSCI441_ARCBALL_CAM_HPP

#include <CSCI441/Camera.hpp>

/**
 * @brief A camera that implements a ArcballCam camera model.
 */
class ArcballCam final : public CSCI441::Camera {
public:
    /**
     * creates a ArcballCam object with the specified perspective projection
     * @param aspectRatio aspect ratio of view plane (defaults to 1.0f)
     * @param fovy vertical field of view (defaults to 45.0f)
     * @param nearClipPlane near z clip plane (defaults to 0.001f)
     * @param farClipPlane far z clip plane (defaults to 1000.0f)
     * @note field of view specified in degrees
     */
    explicit ArcballCam(GLfloat aspectRatio = 1.0f, GLfloat fovy = 45.0f, GLfloat nearClipPlane = 0.001f, GLfloat farClipPlane = 1000.0f);

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

    /// \desc getter and setter for the Arcball radius parameter
    void setRadius(GLfloat radius) {this->_radius = radius;}
    GLfloat getRadius() {return _radius;}

private:
    // updates the look at point and recalculates the view matrix
    void _updateArcballCameraViewMatrix();

    // vertical field of view stored in degrees
    GLfloat _fovy;
    GLfloat _aspectRatio;
    GLfloat _nearClipPlane;
    GLfloat _farClipPlane;

    // distance camera is from look at point
    GLfloat _radius = 3.0f;
};

inline ArcballCam::ArcballCam(
        const GLfloat aspectRatio,
        const GLfloat fovy,
        const GLfloat nearClipPlane,
        const GLfloat farClipPlane
) : _fovy(fovy),
    _aspectRatio(aspectRatio),
    _nearClipPlane(nearClipPlane),
    _farClipPlane(farClipPlane)
{
    mProjectionMatrix = glm::perspective(_fovy, _aspectRatio, _nearClipPlane, _farClipPlane);
}

inline void ArcballCam::recomputeOrientation() {
    // compute position vector based on spherical to cartesian conversion
    mCameraPosition.x = _radius * glm::sin(mCameraTheta ) * glm::sin(mCameraPhi ) + mCameraLookAtPoint.x;
    mCameraPosition.y = _radius * -glm::cos(mCameraPhi ) + mCameraLookAtPoint.y;
    mCameraPosition.z = _radius * -glm::cos(mCameraTheta ) * glm::sin(mCameraPhi ) + mCameraLookAtPoint.z;

    _updateArcballCameraViewMatrix();
}

inline void ArcballCam::moveForward(const GLfloat movementFactor) {
    mCameraPosition += movementFactor; //TODO prob dont want to do this
    mCameraLookAtPoint += movementFactor;
    _updateArcballCameraViewMatrix();
}

inline void ArcballCam::moveBackward(const GLfloat movementFactor) {
    mCameraPosition -= movementFactor;
    mCameraLookAtPoint -= movementFactor;
    _updateArcballCameraViewMatrix();
}

inline void ArcballCam::_updateArcballCameraViewMatrix() {
    computeViewMatrix();
}

#endif // CSCI441_ARCBALL_CAM_HPP