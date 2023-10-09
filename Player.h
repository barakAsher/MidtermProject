//
// Created by eddie on 10/9/23.
//

#ifndef MP_PLAYER_H
#define MP_PLAYER_H

#include <glm/glm.hpp>

class Player {
public:
    // constructor
    Player( glm::vec3 position) {this->_position=position;}

    // getters and setters
    glm::vec3 getPosition() {return  _position;}
    void setPosition(glm::vec3 position) {this->_position = position;}

    // movement functions
    virtual void moveForward();
    virtual void moveBackward();
private:
    glm::vec3 _position;
};

#endif //MP_PLAYER_H
