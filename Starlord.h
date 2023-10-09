//
// Created by forag on 10/4/2023.
//

#ifndef A4_STARLORD_H
#define A4_STARLORD_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "Player.h"


class Starlord : public Player {
public:
    //Starlord(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint materialColorUniformLocation);
    Starlord(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation );
    void drawPlayer( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );


private:

    /// \desc color the plane's body
    glm::vec3 _colorBody;
    /// \desc amount to scale the plane's body by
    glm::vec3 _scaleBody;
    glm::vec3 _translateBody;

    glm::vec3 _colorLeg;
    glm::vec3 _scaleLeg;
    glm::vec3 _translateLegLeft;
    glm::vec3 _translateLegRight;
    GLfloat legAngle;

    glm::vec3 _colorArm;
    glm::vec3 _scaleArm;
    glm::vec3 _translateArmLeft;
    glm::vec3 _translateArmRight;

    glm::vec3 _colorHead;
    glm::vec3 _scaleHead;
    glm::vec3 _translateHead;

    glm::vec3 _colorEye;
    glm::vec3 _scaleEye;
    glm::vec3 _translateEyeLeft;
    glm::vec3 _translateEyeRight;



    void _drawBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
    void _drawLeg(bool leftLeg, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
    void _drawArm(bool lefArm, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
    void _drawHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
    void _drawEye(bool leftEye, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;

};


#endif //A4_STARLORD_H
