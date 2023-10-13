#ifndef MP_VEHICLE_H
#define MP_VEHICLE_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Character.h"

// Vehicle class
class Vehicle : public Character {
public:
    /// \desc creates a simple vehicle that gives the appearance of flight
    /// \param shaderProgramHandle shader program handle that the vehicle should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    Vehicle(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation,GLint modelUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation);

    /// \desc draws the model vehicle for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to vehicle
    /// \param viewMtx camera view matrix to apply to vehicle
    /// \param projMtx camera projection matrix to apply to vehicle
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
    void drawPlayer( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );


private:
    /// \desc current angle of rotation for the propeller
    GLfloat _propAngle;
    /// \desc one rotation step
    GLfloat _propAngleRotationSpeed;
    /// \desc direction plane is facing
    glm::vec3 _forwardDirection;

    /// \desc color the vehicle's body
    glm::vec3 _colorBody;
    /// \desc amount to scale the vehicle's body by
    glm::vec3 _scaleBody;

    /// \desc color the vehicle's wing
    glm::vec3 _colorWing;
    /// \desc amount to scale the vehicle's wing by
    glm::vec3 _scaleWing;
    /// \desc amount to rotate the vehicle's wing by
    GLfloat _rotateWingAngle;

    /// \desc color the vehicle's nose
    glm::vec3 _colorNose;
    /// \desc amount to rotate the vehicle's nose by
    GLfloat _rotateNoseAngle;

    /// \desc color the vehicle's propeller
    glm::vec3 _colorProp;
    /// \desc amount to scale the vehicle's propeller by
    glm::vec3 _scaleProp;
    /// \desc amount to translate the vehicle's propeller by
    glm::vec3 _transProp;

    /// \desc color the vehicle's tail
    glm::vec3 _colorTail;
    glm::vec3 _position;

    const GLfloat _PI = glm::pi<float>();
    const GLfloat _2PI = glm::two_pi<float>();
    const GLfloat _PI_OVER_2 = glm::half_pi<float>();

    /// \desc draws just the vehicle's body
    /// \param modelMtx existing model matrix to apply to vehicle
    /// \param viewMtx camera view matrix to apply to vehicle
    /// \param projMtx camera projection matrix to apply to vehicle
    void _drawVehicleBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws a single wing
    /// \param isLeftWing true if left wing, false if right wing (controls wing rotation)
    /// \param modelMtx existing model matrix to apply to vehicle
    /// \param viewMtx camera view matrix to apply to vehicle
    /// \param projMtx camera projection matrix to apply to vehicle
    void _drawVehicleWing(bool isLeftWing, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the nose of the vehicle
    /// \param modelMtx existing model matrix to apply to vehicle
    /// \param viewMtx camera view matrix to apply to vehicle
    /// \param projMtx camera projection matrix to apply to vehicle
    void _drawVehicleNose(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the two propeller pieces
    /// \param modelMtx existing model matrix to apply to vehicle
    /// \param viewMtx camera view matrix to apply to vehicle
    /// \param projMtx camera projection matrix to apply to vehicle
    void _drawVehiclePropeller(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );
    /// \desc draws the tail of the vehicle
    /// \param modelMtx existing model matrix to apply to vehicle
    /// \param viewMtx camera view matrix to apply to vehicle
    /// \param projMtx camera projection matrix to apply to vehicle
    void _drawVehicleTail(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

};

#endif //MP_VEHICLE_H
