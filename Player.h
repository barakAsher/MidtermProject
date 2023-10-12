//
// Created by eddie on 10/9/23.
//

#ifndef MP_PLAYER_H
#define MP_PLAYER_H

#include <glm/glm.hpp>

class Player {
public:
    Player(){
        _angle = 0;
        _position = {0,0,0};
        _movementFactor = 0.1;
    }
    // getters and setters
    glm::vec3 getPosition() {return  _position;}
    void setPosition(glm::vec3 position) {this->_position = position;}

    GLfloat getAngle() {return _angle;}
    void setAngle(GLfloat angle) {this->_angle = angle;}

    // movement functions
    void moveForward(){_position += glm::vec3(_movementFactor*sin(_angle), 0, -_movementFactor*cos(_angle));}
    void moveBackward(){_position -= glm::vec3(_movementFactor*sin(_angle), 0, -_movementFactor*cos(_angle));}
    void rotate(float amount){_angle += amount;}
    virtual void drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) = 0;

protected:
    /// \desc stores the uniform locations needed for the plan information
    struct ShaderProgramUniformLocations {
        /// \desc location of the precomputed ModelViewProjection matrix
        GLint mvpMtx;
        GLint modelMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normalMtx;
        /// \desc location of the material diffuse color
        GLint materialColor;
    } _shaderProgramUniformLocations;
    /// \desc handle of the shader program to use when drawing the plane
    GLuint _shaderProgramHandle;

    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
        // precompute the Model-View-Projection matrix on the CPU
        glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
        // then send it to the shader on the GPU to apply to every vertex
        glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
        glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.modelMtx, 1, GL_FALSE, &modelMtx[0][0] );
        glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
        glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
    }
private:
    glm::vec3 _position;
    GLfloat _angle;
    float _movementFactor;
};

#endif //MP_PLAYER_H
