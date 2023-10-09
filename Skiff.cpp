#include "Skiff.h"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

Skiff::Skiff(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation ) {

    _propAngle = 0.0f;
    _propAngleRotationSpeed = _PI / 16.0f;

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor    = materialColorUniformLocation;


    _colorBody = glm::vec3( 0.6f, 0.6f, 0.6f );
    _scaleBody = glm::vec3( 8.0f, 3.0f, 4.0f );

    _colorProp = glm::vec3( 1.0f, 1.0f, 1.0f );
    _scaleProp = glm::vec3( 2.2f, 2.0f, 0.5f );
    _transProp = glm::vec3( 0.4f, 0.0f, 0.0f );

}

void Skiff::drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) {
    modelMtx = glm::rotate( modelMtx, -getAngle(), CSCI441::Y_AXIS );
    modelMtx = glm::rotate( modelMtx, getAngle(), CSCI441::Z_AXIS );
    _drawSkiffBody(modelMtx, viewMtx, projMtx);        // the body of our skiff
    _drawSkiffPropeller(true, modelMtx, viewMtx, projMtx);   // left propeller
    _drawSkiffPropeller(false, modelMtx, viewMtx, projMtx);   // right propeller
    rotateProps();
}

void Skiff::rotateProps() {
    _propAngle -= _propAngleRotationSpeed;
    if( _propAngle < 0.0f ) _propAngle += _2PI;
}

void Skiff::_drawSkiffBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::scale( modelMtx, _scaleBody );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorBody[0]);

    CSCI441::drawSolidCube( 0.1f );
}

void Skiff::_drawSkiffPropeller(bool isLeftSide, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    glm::vec3 shiftedTrans = _transProp;
    if (isLeftSide) {
        shiftedTrans.z -= 0.15;
    }
    else {
        shiftedTrans.z += 0.15;
    }
    glm::mat4 modelMtx1 = glm::translate( modelMtx, shiftedTrans );
    modelMtx1 = glm::rotate( modelMtx1, _propAngle, CSCI441::X_AXIS );
    modelMtx1 = glm::scale( modelMtx1, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx1, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorProp[0]);

    CSCI441::drawSolidCube( 0.1f );

    glm::mat4 modelMtx2 = glm::translate( modelMtx, shiftedTrans );
    modelMtx2 = glm::rotate( modelMtx2, _PI_OVER_2 + _propAngle, CSCI441::X_AXIS );
    modelMtx2 = glm::scale( modelMtx2, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx2, viewMtx, projMtx);

    CSCI441::drawSolidCube( 0.1f );
}
