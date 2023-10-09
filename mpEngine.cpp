#include "mpEngine.h"

#include <CSCI441/objects.hpp>

//*************************************************************************************
//
// Helper Functions

#ifndef M_PI
#define M_PI 3.14159265f
#endif

/// \desc Simple helper function to return a random number between 0.0f and 1.0f.
/// \desc Simple helper function to return a random number between 0.0f and 1.0f.
GLfloat getRand() {
    return (GLfloat)rand() / (GLfloat)RAND_MAX;
}

//*************************************************************************************
//
// Public Interface

mpEngine::mpEngine()
         : CSCI441::OpenGLEngine(4, 1,
                                 640, 480,
                                 "MP: The Fellowship") {

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
}

mpEngine::~mpEngine() {
    delete pArcballCam;
//    delete pOverheadCam;
}

void mpEngine::handleKeyEvent(GLint key, GLint action) {
    if(key != GLFW_KEY_UNKNOWN)
        _keys[key] = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));

    if(action == GLFW_PRESS) {
        switch( key ) {
            // quit!
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                setWindowShouldClose();
                break;

            default: break; // suppress CLion warning
        }
    }
}

void mpEngine::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
        // update the left mouse button's state
        _leftMouseButtonState = action;
    }
}

void mpEngine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    // if the left mouse button is being held down while the mouse is moving
    if(_leftMouseButtonState == GLFW_PRESS) {
        // zoom in/out
        if ( _keys[GLFW_KEY_LEFT_SHIFT] || _keys[GLFW_KEY_RIGHT_SHIFT] ) {
            GLfloat radius = pArcballCam->getRadius();
            if(_mousePosition.y > currMousePosition.y) {
                radius += 0.25f;
            }
            else if (_mousePosition.y < currMousePosition.y) {
                radius -= 0.25f;
            }
            // don't zoom in too far
            if (radius > 1.0f) {
                pArcballCam->setRadius(radius);
//                pOverheadCam->setRadius(radius * 5);
            }
            pArcballCam->recomputeOrientation();
//            pOverheadCam->recomputeOrientation();
        }
        else {
            // rotate the camera by the distance the mouse moved
            pArcballCam->rotate((currMousePosition.x - _mousePosition.x) * 0.005f,
                                (_mousePosition.y - currMousePosition.y) * 0.005f );
//            pOverheadCam->rotate((currMousePosition.x - _mousePosition.x) * 0.005f,
//                                (_mousePosition.y - currMousePosition.y) * 0.005f );
        }
    }

    // update the mouse position
    _mousePosition = currMousePosition;
}

//*************************************************************************************
//
// Engine Setup

void mpEngine::mSetupGLFW() {
    CSCI441::OpenGLEngine::mSetupGLFW();

    // set our callbacks
    glfwSetKeyCallback(mpWindow, mp_engine_keyboard_callback);
    glfwSetMouseButtonCallback(mpWindow, mp_engine_mouse_button_callback);
    glfwSetCursorPosCallback(mpWindow, mp_engine_cursor_callback);
}

void mpEngine::mSetupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                        // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	    // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	        // clear the frame buffer to black
}

void mpEngine::mSetupShaders() {
    _lightingShaderProgram = new CSCI441::ShaderProgram("shaders/mp.v.glsl", "shaders/mp.f.glsl" );

    // assign uniforms
    _lightingShaderUniformLocations.mvpMatrix      = _lightingShaderProgram->getUniformLocation("mvpMatrix");
    _lightingShaderUniformLocations.materialColor  = _lightingShaderProgram->getUniformLocation("materialColor");
    _lightingShaderUniformLocations.lightColor     = _lightingShaderProgram->getUniformLocation("lightColor");
    _lightingShaderUniformLocations.lightDirection = _lightingShaderProgram->getUniformLocation("lightDirection");
    _lightingShaderUniformLocations.normalMatrix   = _lightingShaderProgram->getUniformLocation("normalMatrix");

    // assign attributes
    _lightingShaderAttributeLocations.vPos         = _lightingShaderProgram->getAttributeLocation("vPos");
    _lightingShaderAttributeLocations.vNormal      = _lightingShaderProgram->getAttributeLocation(("vNormal"));

}

void mpEngine::mSetupBuffers() {
    //  connect our 3D Object Library to our shader
    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos,_lightingShaderAttributeLocations.vNormal);

    // give the skiff the normal matrix location
    _pSkiff = new Skiff(_lightingShaderProgram->getShaderProgramHandle(),
                        _lightingShaderUniformLocations.mvpMatrix,
                        _lightingShaderUniformLocations.normalMatrix,
                        _lightingShaderUniformLocations.materialColor);
    _createGroundBuffers();
    _generateEnvironment();
}

void mpEngine::_createGroundBuffers() {
    const GLfloat GRID_WIDTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_LENGTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_SPACING_WIDTH = 1.0f;
    const GLfloat GRID_SPACING_LENGTH = 1.0f;
    // precomputed parameters based on above
    const GLfloat LEFT_END_POINT = -GRID_WIDTH / 2.0f - 5.0f;
    const GLfloat RIGHT_END_POINT = GRID_WIDTH / 2.0f + 5.0f;
    const GLfloat BOTTOM_END_POINT = -GRID_LENGTH / 2.0f - 5.0f;
    const GLfloat TOP_END_POINT = GRID_LENGTH / 2.0f + 5.0f;

    struct Vertex {
        GLfloat x, y, z;
        GLfloat xN, yN, zN;

    };
    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;
    // draw horizontal lines
    GLushort currentIndex = 0;
    for(GLfloat i = LEFT_END_POINT; i <= RIGHT_END_POINT; i += GRID_SPACING_WIDTH) {
        Vertex bottomVert = {i, 0.0f, BOTTOM_END_POINT,0,1,0};
        Vertex topVert = {i, 0.0f, TOP_END_POINT ,0,1,0};
        vertices.emplace_back(bottomVert);
        vertices.emplace_back(topVert);
        indices.emplace_back(currentIndex);
        indices.emplace_back(currentIndex + 1);
        currentIndex += 2;
    }
    // draw vertical lines
    for(GLfloat j = BOTTOM_END_POINT; j <= TOP_END_POINT; j += GRID_SPACING_LENGTH) {
        Vertex leftVert = {LEFT_END_POINT, 0.0f, j ,0,1,0};
        Vertex rightVert = {RIGHT_END_POINT, 0.0f, j,0,1,0 };
        vertices.emplace_back(leftVert);
        vertices.emplace_back(rightVert);
        indices.emplace_back(currentIndex);
        indices.emplace_back(currentIndex + 1);
        currentIndex += 2;
    }


    _numGroundPoints = vertices.size();
    //buffer all of the data for the grid to the gpu
    glGenVertexArrays(1, &_groundVAO);
    glBindVertexArray(_groundVAO);

    GLuint vbods[2];       // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vPos);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);


    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vNormal);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
}

void mpEngine::_generateEnvironment() {
    //******************************************************************
    // parameters to make up our grid size and spacing, feel free to
    // play around with this
    const GLfloat GRID_WIDTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_LENGTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_SPACING_WIDTH = 1.0f;
    const GLfloat GRID_SPACING_LENGTH = 1.0f;
    // precomputed parameters based on above
    const GLfloat LEFT_END_POINT = -GRID_WIDTH / 2.0f - 5.0f;
    const GLfloat RIGHT_END_POINT = GRID_WIDTH / 2.0f + 5.0f;
    const GLfloat BOTTOM_END_POINT = -GRID_LENGTH / 2.0f - 5.0f;
    const GLfloat TOP_END_POINT = GRID_LENGTH / 2.0f + 5.0f;
    setLeftEdge(LEFT_END_POINT);
    setRightEdge(RIGHT_END_POINT);
    setTopEdge(TOP_END_POINT);
    setBottomEdge(BOTTOM_END_POINT);
    //******************************************************************

    srand( time(0) );                                                   // seed our RNG

    // psych! everything's on a grid.
    // lava rock color
    glm::vec3 color( 0.5f, 0.4f, 0.4f );
    for(int i = LEFT_END_POINT; i < RIGHT_END_POINT; i += GRID_SPACING_WIDTH) {
        for(int j = BOTTOM_END_POINT; j < TOP_END_POINT; j += GRID_SPACING_LENGTH) {
            // don't just draw a building ANYWHERE.
            if( i % 2 && j % 2 && getRand() < 0.2f ) {
                // translate to spot
                glm::mat4 transToSpotMtx = glm::translate( glm::mat4(1.0), glm::vec3(i, 0.0f, j) );

                // compute random height
                GLdouble height = powf(getRand(), 2.5)*3 + 1;
                // scale to building size
                glm::mat4 scaleToHeightMtx = glm::scale( glm::mat4(1.0), glm::vec3(1, height, 1) );

                // translate up to grid
                glm::mat4 transToHeight = glm::translate( glm::mat4(1.0), glm::vec3(0, height/2.0f, 0) );

                // compute full model matrix
                glm::mat4 modelMatrix = transToHeight * scaleToHeightMtx * transToSpotMtx;

                // store building properties
                BuildingData currentBuilding = {modelMatrix, color};
                _buildings.emplace_back( currentBuilding );
            }
        }
    }
}

void mpEngine::mSetupScene() {
    // set up arcball cam
    pArcballCam = new ArcballCam();
    pArcballCam->setTheta(-M_PI / 1.0f );
    pArcballCam->setPhi(M_PI / 1.5f );
    pArcballCam->setLookAtPoint(_pSkiff->getPosition());
    pArcballCam->recomputeOrientation();

//    // set up overhead cam
//    pOverheadCam = new ArcballCam();
//    pOverheadCam->setTheta(-M_PI);
//    pOverheadCam->setPhi(M_PI-0.001);
//    pOverheadCam->setRadius(pArcballCam->getRadius() * 3);
//    pOverheadCam->recomputeOrientation();

    _cameraSpeed = glm::vec2(0.25f, 0.02f);

    // set lighting uniforms
    glm::vec3 lightDirection(-1.0f, -1.0f, -1.0f);
    glProgramUniform3fv(
            _lightingShaderProgram->getShaderProgramHandle(),
            _lightingShaderUniformLocations.lightDirection,
            1,
            &lightDirection[0]
            );
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glProgramUniform3fv(
            _lightingShaderProgram->getShaderProgramHandle(),
            _lightingShaderUniformLocations.lightColor,
            1,
            &lightColor[0]
            );
}


//*************************************************************************************
//
// Engine Cleanup

void mpEngine::mCleanupShaders() {
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    delete _lightingShaderProgram;
}

void mpEngine::mCleanupBuffers() {
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs();
    glDeleteVertexArrays( 1, &_groundVAO );

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs();

    fprintf( stdout, "[INFO]: ...deleting models..\n" );
    delete _pSkiff;
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void mpEngine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // use our lighting shader program
    _lightingShaderProgram->useProgram();

    //// BEGIN DRAWING THE GROUND PLANE ////
    glm::mat4 groundModelMtx = glm::mat4(1);
    _computeAndSendMatrixUniforms(groundModelMtx, viewMtx, projMtx);

    glm::vec3 groundColor(1.0f,1.0f,1.0f);
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.materialColor, groundColor);

    glBindVertexArray(_groundVAO);
    glDrawElements(GL_LINES, _numGroundPoints, GL_UNSIGNED_SHORT, (void*)0);
    //// END DRAWING THE GROUND PLANE ////

    //// BEGIN DRAWING THE BUILDINGS ////
    for( const BuildingData& currentBuilding : _buildings ) {
        _computeAndSendMatrixUniforms(currentBuilding.modelMatrix, viewMtx, projMtx);

        _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.materialColor, currentBuilding.color);

        CSCI441::drawSolidCube(1.0);
    }
    //// END DRAWING THE BUILDINGS ////

    //// BEGIN DRAWING THE SKIFF ////
    glm::mat4 modelMtx(1.0f);
    // we are going to cheat and use our look at point to place our skiff so that it is always in view

    glm::vec3 drawPoint = _pSkiff -> getPosition();
    drawPoint.y += 0.05;
    modelMtx = glm::translate(modelMtx, drawPoint );

    // rotate the skiff with our camera phi direction
    GLfloat skiffRotate = _pSkiff->getAngle();
    modelMtx = glm::rotate(modelMtx, skiffRotate, CSCI441::Y_AXIS);
    modelMtx = glm::rotate(modelMtx, glm::pi<float>() / 2, CSCI441::X_AXIS );

    // draw our skiff now
    _pSkiff->drawLavaSkiff(modelMtx, viewMtx, projMtx);
    //// END DRAWING THE SKIFF ////
}

void mpEngine::_updateScene() {

    GLfloat skiffMovementSpeed = 0.1f;
    // turn right
    if( _keys[GLFW_KEY_D] || _keys[GLFW_KEY_RIGHT] ) {
        GLfloat currAngle = _pSkiff->getAngle();
        currAngle -= _cameraSpeed.y;
        _pSkiff->setAngle(currAngle);
        pArcballCam->rotate(_cameraSpeed.y, 0.0f);
        pArcballCam->recomputeOrientation();
//        pOverheadCam->rotate(_cameraSpeed.y, 0.0f);
//        pOverheadCam->recomputeOrientation();
        _pSkiff->setPosition(pArcballCam->getLookAtPoint());
    }
    // turn left
    if( _keys[GLFW_KEY_A] || _keys[GLFW_KEY_LEFT] ) {
        GLfloat currAngle = _pSkiff->getAngle();
        currAngle += _cameraSpeed.y;
        _pSkiff->setAngle(currAngle);
        pArcballCam->rotate(-_cameraSpeed.y, 0.0f);
        pArcballCam->recomputeOrientation();
//        pOverheadCam->rotate(-_cameraSpeed.y, 0.0f);
//        pOverheadCam->recomputeOrientation();
        _pSkiff->setPosition(pArcballCam->getLookAtPoint());
    }
    // go forward
    if( _keys[GLFW_KEY_W] || _keys[GLFW_KEY_UP] ) {
        glm::vec3 currPoint = _pSkiff->getPosition();
        glm::vec3 projectedPoint = currPoint;
        projectedPoint.z -= skiffMovementSpeed * cos(_pSkiff->getAngle());
        projectedPoint.x -= skiffMovementSpeed * sin(_pSkiff->getAngle());
        // ensure skiff stays in ground plane
        if (projectedPoint.z > bottomEdge && projectedPoint.z < topEdge && projectedPoint.x < rightEdge && projectedPoint.x > leftEdge) {
            _pSkiff->setPosition(projectedPoint);
            pArcballCam->setLookAtPoint(_pSkiff->getPosition());
            pArcballCam->recomputeOrientation();
//            headCam->setLookAtPoint(projectedPoint);
//            ppOverOverheadCam->recomputeOrientation();
        }
    }
    // go backwards
    if( _keys[GLFW_KEY_S] || _keys[GLFW_KEY_DOWN] ) {
        glm::vec3 currPoint = _pSkiff->getPosition();
        glm::vec3 projectedPoint = currPoint;
        projectedPoint.z += skiffMovementSpeed * cos(_pSkiff->getAngle());
        projectedPoint.x += skiffMovementSpeed * sin(_pSkiff->getAngle());
        // ensure skiff stays in ground plane
        if (projectedPoint.z > bottomEdge && projectedPoint.z < topEdge && projectedPoint.x < rightEdge && projectedPoint.x > leftEdge) {
            _pSkiff->setPosition(projectedPoint);
            pArcballCam->setLookAtPoint(_pSkiff->getPosition());
            pArcballCam->recomputeOrientation();
//            pOverheadCam->setLookAtPoint(projectedPoint);
//            pOverheadCam->recomputeOrientation();
        }
    }

}

void mpEngine::run() {
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(mpWindow) ) {	        // check if the window was instructed to be closed
        glDrawBuffer( GL_BACK );				        // work with our back frame buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        // Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
        // when using a Retina display the actual window can be larger than the requested window.  Therefore,
        // query what the actual size of the window we are rendering to is.
        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( mpWindow, &framebufferWidth, &framebufferHeight );

        // update the viewport - tell OpenGL we want to render to the whole window
        glViewport( 0, 0, framebufferWidth, framebufferHeight );
        // draw everything to the window
        _renderScene(pArcballCam->getViewMatrix(), pArcballCam->getProjectionMatrix());

//        // overhead camera window
//        glClear(GL_DEPTH_BUFFER_BIT);
//        glViewport(framebufferWidth - 200, framebufferHeight - 200, 200, 200);
//        _renderScene(pOverheadCam->getViewMatrix(), pOverheadCam->getProjectionMatrix());

        _updateScene();

        glfwSwapBuffers(mpWindow);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
    }
}

//*************************************************************************************
//
// Private Helper FUnctions

void mpEngine::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.mvpMatrix, mvpMtx);

    // compute and send the normal matrix
    glm::mat3 normalMtx = glm::mat3(glm::transpose(glm::inverse(modelMtx)));
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.normalMatrix, normalMtx);
}

//*************************************************************************************
//
// Callbacks

void mp_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (mpEngine*) glfwGetWindowUserPointer(window);

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void mp_engine_cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (mpEngine*) glfwGetWindowUserPointer(window);

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void mp_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (mpEngine*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}