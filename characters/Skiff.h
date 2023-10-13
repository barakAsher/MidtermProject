#ifndef SKIFF_H
#define SKIFF_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Character.h"

class Skiff : public Character{
public:
    /// \desc creates a simple skiff that gives the appearance of floating
    /// \param shaderProgramHandle shader program handle that the skiff should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    Skiff(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation,GLint modelUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation);

    /// \desc draws the model skiff for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to skiff
    /// \param viewMtx camera view matrix to apply to skiff
    /// \param projMtx camera projection matrix to apply to skiff
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
    void drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) override;

    /// \desc simulates the skiff moving by rotating the propellers
    void rotateProps();

//    /// \desc getter and setter for the angle that the skiff should be drawn at
//    void setSkiffDrawAngle(GLfloat skiffDrawAngle) { this->_skiffDrawAngle = skiffDrawAngle;}
//    GLfloat getSkiffDrawAngle() const {return _skiffDrawAngle;}
//
//    /// \desc getter and setter for the position that the skiff should be drawn at
//    void setPlaneDrawPoint(glm::vec3 skiffDrawPoint) {this->_skiffDrawPoint = skiffDrawPoint;}
//    glm::vec3 getPlaneDrawPoint() {return _skiffDrawPoint;}



private:
    /// \desc position that the skiff should be drawn at
//    glm::vec3 _skiffDrawPoint;
    /// \desc angle that the skiff should be drawn at
//    GLfloat _skiffDrawAngle;

    /// \desc current angle of rotation for the propeller
    GLfloat _propAngle;
    /// \desc one rotation step
    GLfloat _propAngleRotationSpeed;



    /// \desc color the skiff's body
    glm::vec3 _colorBody;
    /// \desc amount to scale the skiff's body by
    glm::vec3 _scaleBody;

    /// \desc color the skiff's propeller
    glm::vec3 _colorProp;
    /// \desc amount to scale the skiff's propeller by
    glm::vec3 _scaleProp;
    /// \desc amount to translate the skiff's propeller by
    glm::vec3 _transProp;

    const GLfloat _PI = glm::pi<float>();
    const GLfloat _2PI = glm::two_pi<float>();
    const GLfloat _PI_OVER_2 = glm::half_pi<float>();

    /// \desc draws just the skiff's body
    /// \param modelMtx existing model matrix to apply to skiff
    /// \param viewMtx camera view matrix to apply to skiff
    /// \param projMtx camera projection matrix to apply to skiff
    void _drawSkiffBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc draws the two propeller pieces
    /// \param modelMtx existing model matrix to apply to skiff
    /// \param viewMtx camera view matrix to apply to skiff
    /// \param projMtx camera projection matrix to apply to skiff
    void _drawSkiffPropeller(bool isLeftSide, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
};

#endif //SKIFF_H