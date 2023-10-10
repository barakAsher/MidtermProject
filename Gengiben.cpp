#include "Gengiben.h"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

Gengiben::Gengiben(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation ) {

    mWobbleAmount = 0.0f;

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor        = materialColorUniformLocation;

}

void Gengiben::drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ){
    modelMtx = glm::translate( modelMtx,getPosition());
    modelMtx = glm::rotate( modelMtx, getAngle() - glm::pi<float>()/2, CSCI441::Y_AXIS );
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
