//
// Created by forag on 10/4/2023.
//

#include "Starlord.h"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

#ifndef M_PI
#define M_PI 3.14159265f
#endif

Starlord::Starlord(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation,GLint modelUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation) {

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor    = materialColorUniformLocation;
    _shaderProgramUniformLocations.modelMtx        = modelUniformLocation;

    _colorBody = glm::vec3( 0.0f, 0.0f, 1.0f );
    _scaleBody = glm::vec3( 2.0f, 1.3f, 1.0f );
    _translateBody = glm::vec3(0.0f,2.0f,0.0f);

    _colorLeg = glm::vec3( 0.59f, 0.29f, 0.0f );
    _scaleLeg = glm::vec3( 0.5f, 1.0f, 0.5f );
    _translateLegLeft = glm::vec3(0.3f,1.0f,0.0f);
    _translateLegRight = glm::vec3(-0.3f,1.0f,0.0f);
    legAngle = 0;

    _colorArm = glm::vec3( 0.82f, 0.7f, 0.55f );
    _scaleArm = glm::vec3( 0.5f, 1.0f, 0.5f );
    _translateArmLeft = glm::vec3(0.75f,1.8f,0.0f);
    _translateArmRight = glm::vec3(-0.75f,1.8f,0.0f);

    _colorHead = glm::vec3( 0.82f, 0.7f, 0.55f );
    _scaleHead = glm::vec3( 0.5f, 0.5f, 0.5f );
    _translateHead = glm::vec3(0.0f,3.7f,0.0f);

    _colorEye = glm::vec3( 0.0f, 0.0f, 0.0f );
    _scaleEye = glm::vec3( 0.45f, 0.45f, 0.45f );
    _translateEyeLeft = glm::vec3(0.25f,3.8f,0.6f);
    _translateEyeRight = glm::vec3(-0.25f,3.8f,0.6f);



}


void Starlord::drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    // idle animation
    legAngle = (legAngle + 0.05);
    if(legAngle > 360) legAngle = 0;
    // actual drawing
    modelMtx = glm::translate(modelMtx, getPosition());
    modelMtx = glm::rotate(modelMtx, getAngle() + glm::pi<float>(), CSCI441::Y_AXIS);
    modelMtx = glm::scale(modelMtx, {.3,.3,.3});
    _drawBody(modelMtx, viewMtx, projMtx);
    _drawLeg(false,modelMtx, viewMtx, projMtx);
    _drawLeg(true,modelMtx, viewMtx, projMtx);
    _drawArm(false,modelMtx, viewMtx, projMtx);
    _drawArm(true,modelMtx, viewMtx, projMtx);
    _drawHead(modelMtx, viewMtx, projMtx);
    _drawEye(false,modelMtx, viewMtx, projMtx);
    _drawEye(true,modelMtx, viewMtx, projMtx);
}

//// Creates the body by translating rotating and scaling a cube from the class models sends the uniform color to the
//// shader files
void Starlord::_drawBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::translate( modelMtx, _translateBody );
    modelMtx = glm::rotate( modelMtx, glm::radians(90.0f),CSCI441::Z_AXIS);
    modelMtx = glm::scale( modelMtx, _scaleBody );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorBody[0]);

    CSCI441::drawSolidCube( 1.0f );
}

//// leftleg and right leg determine where the leg should be translated
void Starlord::_drawLeg(bool leftLeg, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    if(leftLeg){
        modelMtx = glm::translate( modelMtx, _translateLegLeft );
        //// the angle is determing because I had to flip the leg 180 degrees so it rotated about the hip joint instead of around
        //// grid so I used the M_PI and added M_PI/2 * sin(legAngle) to bound the leg angle between -45 and 45 degrees in radians
        //// I subtract for the left leg and add to the right leg so the legs swing in opposite direction
        modelMtx = glm::rotate(modelMtx,(float)M_PI -(float)(M_PI/2*sin(legAngle)),CSCI441::X_AXIS);
    }
    else{
        modelMtx = glm::translate( modelMtx, _translateLegRight );
        modelMtx = glm::rotate(modelMtx,(float)M_PI+ (float)(M_PI/2*sin(legAngle)),CSCI441::X_AXIS);
    }

    modelMtx = glm::scale( modelMtx, _scaleLeg );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorLeg[0]);

    CSCI441::drawSolidCylinder( 0.5, 0.5, 1.0, 16, 16 );
}

void Starlord::_drawArm(bool leftArm, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    if(leftArm) modelMtx = glm::translate( modelMtx, _translateArmLeft );
    else modelMtx = glm::translate( modelMtx, _translateArmRight );

    modelMtx = glm::scale( modelMtx, _scaleArm );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorArm[0]);

    CSCI441::drawSolidCylinder( 0.5, 0.5, 1.0, 16, 16 );
}

void Starlord::_drawHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::translate( modelMtx, _translateHead );
    modelMtx = glm::scale( modelMtx, _scaleHead );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorHead[0]);

    CSCI441::drawSolidSphere( 1.5, 16, 16 );
}

void Starlord::_drawEye(bool leftEye, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    if(leftEye) modelMtx = glm::translate( modelMtx, _translateEyeLeft );
    else modelMtx = glm::translate( modelMtx, _translateEyeRight );

    modelMtx = glm::scale( modelMtx, _scaleEye );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &_colorEye[0]);

    CSCI441::drawSolidSphere( 0.5, 16, 16 );
}