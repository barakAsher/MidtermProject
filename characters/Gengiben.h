#ifndef LAB05_PLANE_H
#define LAB05_PLANE_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "Character.h"

class Gengiben: public Character {
public:
    /// \desc creates a simple plane that gives the appearance of flight
    /// \param shaderProgramHandle shader program handle that the plane should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    Gengiben(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation,GLint modelUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation);
    void drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) override;

private:
    void mDrawBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );
    void mDrawEye(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx, bool isLeft);
    void mDrawEyebrow(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx, bool isLeft );

    glm::vec3 mBodyColor;
    glm::vec3 mHatColor;
    glm::vec3 mEyeColor;
    glm::vec3 mPupilColor;
    glm::vec3 mEyebrowColor;


    float mWobbleAmount;
    int mWobbleDir = 1;

    const GLfloat _PI = glm::pi<float>();
    const GLfloat _2PI = glm::two_pi<float>();
    const GLfloat _PI_OVER_2 = glm::half_pi<float>();

};


#endif //LAB05_PLANE_H
