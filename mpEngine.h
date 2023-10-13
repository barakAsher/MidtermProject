#ifndef LAB05_LAB05_ENGINE_H
#define LAB05_LAB05_ENGINE_H

#include <vector>

#include <CSCI441/OpenGLEngine.hpp>
#include <CSCI441/ShaderProgram.hpp>

#include "ArcballCam.hpp"
#include "CSCI441/FreeCam.hpp"
#include "CSCI441/FixedCam.hpp"

#include "characters/Character.h"
#include "characters/Skiff.h"
#include "characters/Vehicle.h"
#include "characters/Starlord.h"
#include "characters/Gengiben.h"
#include "FirstPersonCam.h"

class mpEngine final : public CSCI441::OpenGLEngine {
public:
    mpEngine();
    ~mpEngine();

    void run() final;

    /// \desc handle any key events inside the engine
    /// \param key key as represented by GLFW_KEY_ macros
    /// \param action key event action as represented by GLFW_ macros
    void handleKeyEvent(GLint key, GLint action);

    /// \desc handle any mouse button events inside the engine
    /// \param button mouse button as represented by GLFW_MOUSE_BUTTON_ macros
    /// \param action mouse event as represented by GLFW_ macros
    void handleMouseButtonEvent(GLint button, GLint action);

    /// \desc handle any cursor movement events inside the engine
    /// \param currMousePosition the current cursor position
    void handleCursorPositionEvent(glm::vec2 currMousePosition);

    /// \desc value off-screen to represent mouse has not begun interacting with window yet
    static constexpr GLfloat MOUSE_UNINITIALIZED = -9999.0f;

    /// \desc getters and setters for the bounds of the ground plane
    void setRightEdge(GLfloat rightEdge) {this->rightEdge = rightEdge;}
    void setLeftEdge(GLfloat leftEdge) {this->leftEdge = leftEdge;}
    void setTopEdge(GLfloat topEdge) {this->topEdge = topEdge;}
    void setBottomEdge(GLfloat bottomEdge) {this->bottomEdge = bottomEdge;}
    GLfloat getRightEdge() const {return rightEdge;}
    GLfloat getLeftEdge() const {return leftEdge;}
    GLfloat getTopEdge() const {return topEdge;}
    GLfloat getBottomEdge() const {return bottomEdge;}


private:
    void mSetupGLFW() final;
    void mSetupOpenGL() final;
    void mSetupShaders() final;
    void mSetupBuffers() final;
    void mSetupScene() final;

    void mCleanupBuffers() final;
    void mCleanupShaders() final;

    /// \desc draws everything to the scene from a particular point of view
    /// \param viewMtx the current view matrix for our camera
    /// \param projMtx the current projection matrix for our camera
    void _renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const;
    /// \desc handles moving our ArcballCam as determined by keyboard input
    void _updateScene();

    /// \desc tracks the number of different keys that can be present as determined by GLFW
    static constexpr GLuint NUM_KEYS = GLFW_KEY_LAST;
    /// \desc boolean array tracking each key state.  if true, then the key is in a pressed or held
    /// down state.  if false, then the key is in a released state and not being interacted with
    GLboolean _keys[NUM_KEYS];

    /// \desc last location of the mouse in window coordinates
    glm::vec2 _mousePosition;
    /// \desc current state of the left mouse button
    GLint _leftMouseButtonState;

    /// \desc the arcball camera in our world
    ArcballCam* pArcballCam;
    CSCI441::FreeCam* pFreeCam;
    FirstPersonCamera* fpCam;


    int _currentCam;
    bool _fpCamShown;

    /// \desc pair of values to store the speed the camera can move/rotate.
    /// \brief x = forward/backward delta, y = rotational delta
    glm::vec2 _cameraSpeed;


    /// \desc edges of our ground plane
    GLfloat leftEdge;
    GLfloat rightEdge;
    GLfloat topEdge;
    GLfloat bottomEdge;

    std::vector<Character*> _players;
    GLint _currentPlayerIdx;
    Character* _currentPlayer;

    /// \desc the size of the world (controls the ground size and locations of buildings)
    static constexpr GLfloat WORLD_SIZE = 55.0f;
    /// \desc VAO for our ground
    GLuint _groundVAO;
    /// \desc the number of points that make up our ground object
    GLsizei _numGroundPoints;

    /// \desc creates the ground VAO
    void _createGroundBuffers();

    /// \desc smart container to store information specific to each building we wish to draw
    struct BuildingData {
        /// \desc transformations to position and size the building
        glm::mat4 modelMatrix;
        /// \desc color to draw the building
        glm::vec3 color;
    };

    struct MushroomData {
        /// \desc transformations to position and size the building
        glm::mat4 modelMatrix;
        /// \desc color to draw the building
        glm::vec3 color;
    };
    /// \desc information list of all the buildings to draw
    std::vector<BuildingData> _buildings;
    std::vector<MushroomData> _mushrooms;


    /// \desc generates building information to make up our scene
    void _generateEnvironment();

    /// \desc shader program that performs lighting
    CSCI441::ShaderProgram* _lightingShaderProgram = nullptr;   // the wrapper for our shader program
    /// \desc stores the locations of all of our shader uniforms
    struct LightingShaderUniformLocations {
        /// \desc precomputed MVP matrix location
        GLint mvpMatrix;
        GLint modelMtx;
        GLint numDirLights;
        GLint numPointLights;
        GLint numSpotLights;
        /// \desc material diffuse color location
        GLint materialColor;
        GLint normalMatrix;
        GLint lookAtDir;
        GLint alpha;

    } _lightingShaderUniformLocations;

    //info for directional lights
    struct DirectionalLight{
        glm::vec3 direction;
        glm::vec3 color;
        float intensity;
    };
    //info for point lights
    struct PointLight{
        glm::vec3 position;
        glm::vec3 color;
        GLfloat linAtten;
        GLfloat quadAtten;
        GLfloat expAtten;
    };
    //info for spotlights
    struct SpotLight{
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 color;
        GLfloat angle;
        GLfloat linAtten;
        GLfloat quadAtten;
        GLfloat expAtten;
    };

    std::vector<DirectionalLight> _dirLights;
    std::vector<PointLight> _pointLights;
    std::vector<SpotLight> _spotLights;

    void _setupLights();

    struct DirectionalLightUniformLocations{
        std::vector<GLint> directionLocs;
        std::vector<GLint> colorLocs;
        std::vector<float> intensityLocs;

    } _dirLightUniformLocations;
    struct PointLightUniformLocations{
        std::vector<GLint> positionLocs;
        std::vector<GLint> colorLocs;
        std::vector<GLint> linearAttenLocs;
        std::vector<GLint> quadAttenLocs;
        std::vector<GLint> expAttenLocs;
    } _pointLightUniformLocations;

    struct SpotLightUniformLocations{
        std::vector<GLint> positionLocs;
        std::vector<GLint> colorLocs;
        std::vector<GLint> dirLocs;
        std::vector<GLint> angleLocs;
        std::vector<GLint> linearAttenLocs;
        std::vector<GLint> quadAttenLocs;
        std::vector<GLint> expAttenLocs;
    } _spotLightUniformLocations;

    /// \desc stores the locations of all of our shader attributes
    struct LightingShaderAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
        GLint vNormal;

    } _lightingShaderAttributeLocations;

    GLint _lightType;
    float _alpha;

    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;

};

void mp_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods );
void mp_engine_cursor_callback(GLFWwindow *window, double x, double y );
void mp_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods );

#endif// LAB05_LAB05_ENGINE_H
