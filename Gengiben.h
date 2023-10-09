#ifndef LAB05_PLANE_H
#define LAB05_PLANE_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class Gengiben {
public:
    /// \desc creates a simple plane that gives the appearance of flight
    /// \param shaderProgramHandle shader program handle that the plane should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    Gengiben(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation);

    /// \desc draws the model plane for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
    void drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );


    /// \desc simulates the plane flying forward by rotating the propeller clockwise
    void goForward();
    /// \desc simulates the plane flying backward by rotating the propeller counter-clockwise
    void goBackward();

    void rotate(float amount);

    void setPos(glm::vec3 newPos);
    glm::vec3 getPos();
    void setRot(float newDir);
    float getRot();
    void setMoveFactor(float newDir);
    float getMoveFactor();


private:
    void mDrawBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );
    void mDrawEye(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx, bool isLeft);
    void mDrawEyebrow(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx, bool isLeft );

    glm::vec3 mBodyColor;
    glm::vec3 mHatColor;
    glm::vec3 mEyeColor;
    glm::vec3 mPupilColor;
    glm::vec3 mEyebrowColor;
    /// \desc handle of the shader program to use when drawing the plane
    GLuint _shaderProgramHandle;
    /// \desc stores the uniform locations needed for the plan information
    struct ShaderProgramUniformLocations {
        /// \desc location of the precomputed ModelViewProjection matrix
        GLint mvpMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normalMtx;
        /// \desc location of the material diffuse color
        GLint materialColor;
    } _shaderProgramUniformLocations;

    /// \desc angle to rotate our plane at
    GLfloat _rotatePlaneAngle;

    glm::vec3 mPos;
    float mRot;
    float mMovementFactor;
    float mWobbleAmount;
    int mWobbleDir = 1;

    const GLfloat _PI = glm::pi<float>();
    const GLfloat _2PI = glm::two_pi<float>();
    const GLfloat _PI_OVER_2 = glm::half_pi<float>();


    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
};


#endif //LAB05_PLANE_H
