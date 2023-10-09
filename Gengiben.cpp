#include "Gengiben.h"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

Gengiben::Gengiben(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation ) {
    mPos = {53,2,50};
    mRot = 0;
    mMovementFactor = 0.1;
    mWobbleAmount = 0.0f;

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor        = materialColorUniformLocation;

    _rotatePlaneAngle = _PI / 2.0f;

}

void Gengiben::drawGengiben(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) {
    modelMtx = glm::rotate( modelMtx, _rotatePlaneAngle, CSCI441::Y_AXIS );
    modelMtx = glm::rotate( modelMtx, mWobbleAmount, CSCI441::X_AXIS );
    mDrawBody(modelMtx, viewMtx, projMtx);

    mWobbleAmount  +=0.01f*mWobbleDir;
    if(mWobbleAmount>_PI/4 || mWobbleAmount < -_PI/4)
        mWobbleDir*=-1;
}
void Gengiben::mDrawBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ){
    mBodyColor = {.5,.5,.5};
    modelMtx = glm::scale(modelMtx, {1,1.5,1});
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &mBodyColor[0]);
    CSCI441::drawSolidSphere(.4,10,10);

    modelMtx = glm::scale(modelMtx, {1.5,1.0f,1.5});
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    modelMtx = glm::translate(modelMtx, {-.25,.25,.1});
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    mDrawEye(modelMtx, viewMtx, projMtx, true);

    modelMtx = glm::translate(modelMtx, {0,0,-.2});
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    mDrawEye(modelMtx, viewMtx, projMtx, false);
}

void Gengiben::mDrawEye(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx, bool isLeft) {
    mEyeColor = {1,1,1};
    mPupilColor = {0,0,0};
    modelMtx = glm::scale(modelMtx,{1.5,1.5,1.5});
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &mEyeColor[0]);
    CSCI441::drawSolidSphere(.05, 10,10);

    //draw the pupils
    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &mPupilColor[0]);
    modelMtx = glm::translate(modelMtx,{-.04,0,0});
    modelMtx = glm::scale(modelMtx,{.3,.3,.3});
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    CSCI441::drawSolidSphere(.05, 10,10);

    modelMtx = glm::translate(modelMtx, {.2,.2,0});
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    modelMtx = glm::scale(modelMtx, {1,.5,1.5});
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    mDrawEyebrow(modelMtx, viewMtx,projMtx,isLeft);
}

void Gengiben::mDrawEyebrow(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx, bool isLeft) {
    mEyebrowColor = {0.49, 0.255, 0.098};
    modelMtx = glm::rotate(modelMtx, isLeft?1.0f:-1.0f, CSCI441::X_AXIS);
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, &mEyebrowColor[0]);
    CSCI441::drawSolidCube(.2);
}

void Gengiben::goForward() {
    mPos += glm::vec3(mMovementFactor*sin(mRot), 0, -mMovementFactor*cos(mRot));
}

void Gengiben::goBackward() {
    mPos -= glm::vec3(mMovementFactor*sin(mRot), 0, -mMovementFactor*cos(mRot));
}


void Gengiben::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );

    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}
void Gengiben::rotate(float amount){
    mRot += amount;
}
glm::vec3 Gengiben::getPos() {
    return mPos;
}
float Gengiben::getRot() {
    return mRot;
}
void Gengiben::setPos(glm::vec3 newPos) {
    mPos = newPos;
}
void Gengiben::setRot(float newDir) {
    mRot = newDir;
}

float Gengiben::getMoveFactor() {
    return mMovementFactor;
}
void Gengiben::setMoveFactor(float newFactor) {
    mMovementFactor = newFactor;
}