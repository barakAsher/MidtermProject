//
// Created by eddie on 10/9/23.
//

#ifndef MP_PLAYER_H
#define MP_PLAYER_H

#include <glm/glm.hpp>

class Player {
public:
    // getters and setters
    glm::vec3 getPosition() {return  _position;}
    void setPosition(glm::vec3 position) {this->_position = position;}

    GLfloat getAngle() {return _angle;}
    void setAngle(GLfloat angle) {this->_angle = angle;}

    // movement functions
    virtual void drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) = 0;
//    virtual void moveBackward();
private:
    glm::vec3 _position;
    GLfloat _angle;
};

#endif //MP_PLAYER_H
