#include "Vehicle.h"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

Vehicle::Vehicle(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation,GLint modelUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation) {
    _propAngle = 0.0f;

    _propAngleRotationSpeed = _PI / 16.0f;

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor    = materialColorUniformLocation;
    _shaderProgramUniformLocations.modelMtx        = modelUniformLocation;

    _forwardDirection = glm::vec3(1.0f, 0.0f, 0.0f);


    _colorBody = glm::vec3( 0.1f, 0.6f, 0.9f );
    //_scaleBody = glm::vec3( 2.0f, 0.5f, 1.0f );
    _scaleBody = glm::vec3( 1.0f, 1.0f, 1.0f );

    _position = glm::vec3(0.0f, 0.5f, 0.0f); //position of where the plane is drawn


    _colorWing = glm::vec3( 0.5f, 0.2f, 0.5f );
    _scaleWing = glm::vec3( 1.5f, 0.5f, 1.0f );
    _rotateWingAngle = _PI / 2.0f;

    _colorNose = glm::vec3( 0.8f, 0.7f, 0.3f );
    _rotateNoseAngle = _PI / 3.0f;

    _colorProp = glm::vec3( 1.0f, 1.0f, 1.0f );
    _scaleProp = glm::vec3( 1.1f, 1.0f, 0.025f );
    _transProp = glm::vec3( 0.1f, 0.0f, 0.0f );

    _colorTail = glm::vec3( 1.0f, 1.0f, 0.0f );
}

void Vehicle::drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {

    //Ensure that the the plane is drawn correctly after it has been rotated
    modelMtx = glm::translate(modelMtx, getPosition());
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), getAngle()-glm::pi<float>()/2, CSCI441::Y_AXIS);
    modelMtx = modelMtx * rotationMatrix;


    _drawPlaneBody(modelMtx, viewMtx, projMtx);        // the body of our plane
    _drawPlaneWing(true, modelMtx, viewMtx, projMtx);  // the left wing
    _drawPlaneWing(false, modelMtx, viewMtx, projMtx); // the right wing
    _drawPlaneNose(modelMtx, viewMtx, projMtx);        // the nose
    _drawPlanePropeller(modelMtx, viewMtx, projMtx);   // the propeller
    _drawPlaneTail(modelMtx, viewMtx, projMtx);        // the tail
}


void Vehicle::_drawPlaneBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {

    modelMtx = glm::scale( modelMtx, _scaleBody );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorBody[0]);

    //CSCI441::drawSolidCube( 0.1f );
    CSCI441::drawSolidSphere(0.18, 50, 3);
}

void Vehicle::_drawPlaneWing(bool isLeftWing, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::scale( modelMtx, _scaleWing );
    modelMtx = glm::rotate( modelMtx, (isLeftWing ? -1.f : 1.f) * _rotateWingAngle, CSCI441::X_AXIS );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorWing[0]);

    //CSCI441::drawSolidCone( 0.05f, 0.2f, 16, 16 );
    CSCI441::drawSolidSphere(0.2, 50, 3);
}

void Vehicle::_drawPlaneNose(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    //modelMtx = glm::scale( modelMtx, _scaleBody );
    modelMtx = glm::rotate( modelMtx, _rotateNoseAngle, CSCI441::Z_AXIS );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorNose[0]);

    CSCI441::drawSolidCone( 0.025f, 0.3f, 16, 16 );
    //CSCI441::drawSolidSphere(0.2, 50, 3);
}

void Vehicle::_drawPlanePropeller(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    glm::mat4 modelMtx1 = glm::translate( modelMtx, _transProp );
    modelMtx1 = glm::rotate( modelMtx1, _propAngle, CSCI441::X_AXIS );
    modelMtx1 = glm::scale( modelMtx1, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx1, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorProp[0]);

    CSCI441::drawSolidCube( 0.1f );

    glm::mat4 modelMtx2 = glm::translate( modelMtx, _transProp );
    modelMtx2 = glm::rotate( modelMtx2, _PI_OVER_2 + _propAngle, CSCI441::X_AXIS );
    modelMtx2 = glm::scale( modelMtx2, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx2, viewMtx, projMtx);

    CSCI441::drawSolidCube( 0.1f );
}

void Vehicle::_drawPlaneTail(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorTail[0]);

    CSCI441::drawSolidCone( 0.02f, 0.1f, 16, 16 );
}
