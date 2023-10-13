//
// Created by forag on 10/13/2023.
//

#include "FirstPersonCam.h"

FirstPersonCam::FirstPersonCam(
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

inline void FirstPersonCam::recomputeOrientation() {
    // compute direction vector based on spherical to cartesian conversion
    mCameraDirection.x =  glm::sin(mCameraTheta ) * glm::sin(mCameraPhi );
    mCameraDirection.y = -glm::cos(mCameraPhi );
    mCameraDirection.z = -glm::cos(mCameraTheta ) * glm::sin(mCameraPhi );

    // and normalize this directional vector!
    mCameraDirection = glm::normalize(mCameraDirection );

    _updateFreeCam2CameraViewMatrix();
}

inline void FirstPersonCam::moveForward(const GLfloat movementFactor) {
    mCameraPosition += movementFactor; //TODO prob dont want to do this
    mCameraLookAtPoint += movementFactor;
    _updateFreeCam2CameraViewMatrix();
}

inline void FirstPersonCam::moveBackward(const GLfloat movementFactor) {
    mCameraPosition -= movementFactor;
    mCameraLookAtPoint -= movementFactor;
    _updateFreeCam2CameraViewMatrix();
}

inline void FirstPersonCam::_updateFreeCam2CameraViewMatrix() {
    setLookAtPoint(mCameraPosition + mCameraDirection );
    computeViewMatrix();
}
