#include "mpEngine.h"
#include "iostream"

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
    _currentPlayerIdx = 0;
    _currentCam = 1;
    _fpCamShown = false;
}

mpEngine::~mpEngine() {
    delete pArcballCam;
    delete pFreeCam;
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

            case GLFW_KEY_LEFT_CONTROL:
                _currentPlayerIdx++;
                if (_currentPlayerIdx > _players.size() - 1)
                    _currentPlayerIdx = 0;
                pArcballCam->setTheta(glm::pi<float>() - _players[_currentPlayerIdx]->getAngle());
                break;
            case GLFW_KEY_1:
                _currentCam=1;
                break;
            case GLFW_KEY_2:
                _currentCam=2;
                break;
            case GLFW_KEY_3:
                _fpCamShown = !_fpCamShown;
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
        if(_currentCam == 1){   //move arcball
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
                }
                pArcballCam->recomputeOrientation();
            }
            else {
                // rotate the camera by the distance the mouse moved
                pArcballCam->rotate((currMousePosition.x - _mousePosition.x) * 0.005f,
                                    (_mousePosition.y - currMousePosition.y) * 0.005f );
            }
        }
        else{
            glm::vec2 mouseChange = (currMousePosition-_mousePosition);
            pFreeCam->rotate(mouseChange.x*0.005f,-mouseChange.y*0.005f);
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
void mpEngine::_setupLights(){
    _lightingShaderUniformLocations.numPointLights = _lightingShaderProgram->getUniformLocation("numPointLights");
    _lightingShaderUniformLocations.numDirLights = _lightingShaderProgram->getUniformLocation("numDirLights");
    _lightingShaderUniformLocations.numSpotLights = _lightingShaderProgram->getUniformLocation("numSpotLights");
    PointLight firstPoint{{0,2,0},{1,1,1},0,.2f,0};
    PointLight secondPoint{{30,2,30},{1,1,1},0,.2f,0};
    _pointLights.emplace_back(firstPoint);
    _pointLights.emplace_back(secondPoint);

    DirectionalLight firstDir{{-1,-1,-1},{1,1,1},.2};
    _dirLights.emplace_back(firstDir);

    SpotLight firstSpot{{0,10,0},{0,-1,0},{1,0,1},1, 0,.2,0};
    _spotLights.emplace_back(firstSpot);

    int numDirLights = _dirLights.size();
    glProgramUniform1iv(_lightingShaderProgram->getShaderProgramHandle(),_lightingShaderUniformLocations.numDirLights,1,&numDirLights);
    int numPointLights = _pointLights.size();
    glProgramUniform1iv(_lightingShaderProgram->getShaderProgramHandle(),_lightingShaderUniformLocations.numPointLights,1,&numPointLights);
    int numSpotLights = _spotLights.size();
    glProgramUniform1iv(_lightingShaderProgram->getShaderProgramHandle(),_lightingShaderUniformLocations.numSpotLights,1,&numSpotLights);
}

void mpEngine::mSetupShaders() {
    _lightingShaderProgram = new CSCI441::ShaderProgram("shaders/mp.v.glsl", "shaders/mp.f.glsl" );

    // assign uniforms
    _lightingShaderUniformLocations.mvpMatrix      = _lightingShaderProgram->getUniformLocation("mvpMatrix");
    _lightingShaderUniformLocations.modelMtx      = _lightingShaderProgram->getUniformLocation("modelMtx");
    _lightingShaderUniformLocations.materialColor  = _lightingShaderProgram->getUniformLocation("materialColor");
    _lightingShaderUniformLocations.normalMatrix   = _lightingShaderProgram->getUniformLocation("normalMatrix");
    _lightingShaderUniformLocations.lookAtDir    = _lightingShaderProgram->getUniformLocation("lookAtDir");
    _lightingShaderUniformLocations.alpha   = _lightingShaderProgram->getUniformLocation("alpha");
    // assign attributes
    _lightingShaderAttributeLocations.vPos         = _lightingShaderProgram->getAttributeLocation("vPos");
    _lightingShaderAttributeLocations.vNormal      = _lightingShaderProgram->getAttributeLocation(("vNormal"));

    _setupLights();
    //need to set pointlight size here
    for(int i=0;i<_pointLights.size();i++){
        std::string test = "pointLights["+std::to_string(i)+"].pos";
        _pointLightUniformLocations.positionLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "pointLights["+std::to_string(i)+"].color";
        _pointLightUniformLocations.colorLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "pointLights["+std::to_string(i)+"].atten.lin";
        _pointLightUniformLocations.linearAttenLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "pointLights["+std::to_string(i)+"].atten.quad";
        _pointLightUniformLocations.quadAttenLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "pointLights["+std::to_string(i)+"].atten.exp";
        _pointLightUniformLocations.expAttenLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
    }

    for(int i=0;i<_dirLights.size();i++){
        std::string test = "dirLights["+std::to_string(i)+"].direction";
        _dirLightUniformLocations.directionLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "dirLights["+std::to_string(i)+"].color";
        _dirLightUniformLocations.colorLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "dirLights["+std::to_string(i)+"].intensity";
        _dirLightUniformLocations.intensityLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
    }

    for(int i=0;i<_spotLights.size();i++){
        std::string test = "spotLights["+std::to_string(i)+"].pos";
        _spotLightUniformLocations.positionLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "spotLights["+std::to_string(i)+"].color";
        _spotLightUniformLocations.colorLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "spotLights["+std::to_string(i)+"].dir";
        _spotLightUniformLocations.dirLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "spotLights["+std::to_string(i)+"].angle";
        _spotLightUniformLocations.angleLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "spotLights["+std::to_string(i)+"].atten.lin";
        _spotLightUniformLocations.linearAttenLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "spotLights["+std::to_string(i)+"].atten.quad";
        _spotLightUniformLocations.quadAttenLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
        test = "spotLights["+std::to_string(i)+"].atten.exp";
        _spotLightUniformLocations.expAttenLocs.emplace_back(_lightingShaderProgram->getUniformLocation(test.c_str()));
    }

}

void mpEngine::mSetupBuffers() {
    //  connect our 3D Object Library to our shader
    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos,_lightingShaderAttributeLocations.vNormal);

    // give the skiff the normal matrix location
    Player* pSkiff = new Skiff(_lightingShaderProgram->getShaderProgramHandle(),
                        _lightingShaderUniformLocations.mvpMatrix,
                        _lightingShaderUniformLocations.modelMtx,
                        _lightingShaderUniformLocations.normalMatrix,
                        _lightingShaderUniformLocations.materialColor);
    pSkiff->setPosition({0,0,0});
    _players.push_back(pSkiff);

    Player* pVehicle = new Vehicle(_lightingShaderProgram->getShaderProgramHandle(),
                                   _lightingShaderUniformLocations.mvpMatrix,
                                   _lightingShaderUniformLocations.modelMtx,
                                   _lightingShaderUniformLocations.normalMatrix,
                                   _lightingShaderUniformLocations.materialColor);

    pVehicle->setPosition({1,0,0});
    _players.push_back((pVehicle));

    Player* pStarlord = new Starlord(_lightingShaderProgram->getShaderProgramHandle(),
                                     _lightingShaderUniformLocations.mvpMatrix,
                                     _lightingShaderUniformLocations.modelMtx,
                                     _lightingShaderUniformLocations.normalMatrix,
                                     _lightingShaderUniformLocations.materialColor);
    pStarlord->setPosition({2,0,0});
    _players.push_back(pStarlord);

    Player* pGengiben = new Gengiben(_lightingShaderProgram->getShaderProgramHandle(),
                                     _lightingShaderUniformLocations.mvpMatrix,
                                     _lightingShaderUniformLocations.modelMtx,
                                     _lightingShaderUniformLocations.normalMatrix,
                                     _lightingShaderUniformLocations.materialColor);
    pGengiben->setPosition({3,1,0});
    _players.push_back(pGengiben);


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
//    for(GLfloat i = LEFT_END_POINT; i <= RIGHT_END_POINT; i += GRID_SPACING_WIDTH) {
//        Vertex bottomVert = {i, 0.0f, BOTTOM_END_POINT,0,1,0};
//        Vertex topVert = {i, 0.0f, TOP_END_POINT ,0,1,0};
//        vertices.emplace_back(bottomVert);
//        vertices.emplace_back(topVert);
//        indices.emplace_back(currentIndex);
//        indices.emplace_back(currentIndex + 1);
//        currentIndex += 2;
//    }
//    // draw vertical lines
//    for(GLfloat j = BOTTOM_END_POINT; j <= TOP_END_POINT; j += GRID_SPACING_LENGTH) {
//        Vertex leftVert = {LEFT_END_POINT, 0.0f, j ,0,1,0};
//        Vertex rightVert = {RIGHT_END_POINT, 0.0f, j,0,1,0 };
//        vertices.emplace_back(leftVert);
//        vertices.emplace_back(rightVert);
//        indices.emplace_back(currentIndex);
//        indices.emplace_back(currentIndex + 1);
//        currentIndex += 2;
//    }

    for(float i=LEFT_END_POINT;i<=RIGHT_END_POINT;i+=GRID_SPACING_WIDTH){
        for(float j=BOTTOM_END_POINT;j<=TOP_END_POINT;j+=GRID_SPACING_LENGTH){
            Vertex first = {i,0,j,0,1,0};
            vertices.emplace_back(first);
        }
    }
    for(float i=LEFT_END_POINT;i<=RIGHT_END_POINT;i+=GRID_SPACING_WIDTH){
        for(float j=BOTTOM_END_POINT;j<=TOP_END_POINT;j+=GRID_SPACING_LENGTH){
            Vertex first = {i,0,j,0,1,0};
            vertices.emplace_back(first);
        }
    }
    int numCols = (RIGHT_END_POINT-LEFT_END_POINT)/GRID_SPACING_WIDTH +1;
    int numRows = (TOP_END_POINT-BOTTOM_END_POINT)/GRID_SPACING_LENGTH+1;

    for(int i=0;i<numCols;i++){
        for(int j=0;j<numRows ;j++){
            indices.emplace_back(i*numCols+j);
        }
    }
    for(int i=0;i<numCols;i++){
        for(int j=0;j<numRows;j++){
            indices.emplace_back(j*numCols+i);
        }
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

    srand( glfwGetTime() );// seed our RNG

    glm::vec3 color(0.5f, 0.4f, 0.4f);
    glm::vec3 mushColor(1.0f, 0.4f, 0.4f);
    // psych! everything's on a grid.
    // lava rock color
    for (int i = LEFT_END_POINT; i < RIGHT_END_POINT; i += GRID_SPACING_WIDTH) {
        for (int j = BOTTOM_END_POINT; j < TOP_END_POINT; j += GRID_SPACING_LENGTH) {
            // Determine if a building should be placed
            bool placeBuilding = i % 2 && j % 2 && getRand() < 0.2f;

            // Translate to the spot (for both building and mushroom)
            glm::mat4 transToSpotMtx = glm::translate(glm::mat4(1.0), glm::vec3(i, 0.0f, j));

            if (placeBuilding) {
                // Compute random height for building
                GLdouble height = powf(getRand(), 2.5) * 3 + 1;

                // Scale to building size
                glm::mat4 scaleToHeightMtx = glm::scale(glm::mat4(1.0), glm::vec3(1, height, 1));

                // Translate up to grid
                glm::mat4 transToHeight = glm::translate(glm::mat4(1.0), glm::vec3(0, height / 2.0f, 0));

                // Compute full model matrix for building
                glm::mat4 modelMatrix = transToHeight * scaleToHeightMtx * transToSpotMtx;

                // Store building properties
                BuildingData currentBuilding = {modelMatrix, color};
                _buildings.emplace_back(currentBuilding);
            } if (i % 2 && j % 2 && getRand() < 0.05f)  { //Adjust the value to the right of the < sign for more or less mushrooms
                // For mushrooms, generate a random position within the grid cell
                // For mushrooms, generate a random position within the grid cell
                float mushPosX = i + (getRand() * (GRID_SPACING_WIDTH - 0.2f)) + 0.1f;
                float mushPosZ = j + (getRand() * (GRID_SPACING_LENGTH - 0.2f)) + 0.1f;

                // Translate to the mushroom's random position
                glm::mat4 transToMushroomSpotMtx = glm::translate(glm::mat4(1.0), glm::vec3(mushPosX, 0.0f, mushPosZ));

                // Compute full model matrix for mushroom
                glm::mat4 modelMatrixMushroom = transToMushroomSpotMtx;

                // Store mushroom properties
                MushroomData currentMushroomData = {modelMatrixMushroom, mushColor};
                _mushrooms.emplace_back(currentMushroomData);
            }
        }
    }
}

void mpEngine::mSetupScene() {
    // set up arcball cam
    _currentPlayer = _players[_currentPlayerIdx];
    pArcballCam = new ArcballCam();
    pArcballCam->setTheta(M_PI );
    pArcballCam->setPhi(M_PI * 2 / 2.8f );
    pArcballCam->setRadius(5);
    pArcballCam->setLookAtPoint(_currentPlayer->getPosition());
    pArcballCam->recomputeOrientation();

    fpCam = new FirstPersonCamera(pArcballCam->getLookAtPoint());

    pFreeCam = new CSCI441::FreeCam();
    pFreeCam->setPosition(glm::vec3(30.0f, 20.0f, 15.0f) );
    pFreeCam->setTheta(-M_PI / 3.0f );
    pFreeCam->setPhi(M_PI / 2.8f );
    pFreeCam->recomputeOrientation();

    _cameraSpeed = glm::vec2(0.25f, 0.02f);

    _alpha = .001f;

    glProgramUniform1fv(
            _lightingShaderProgram->getShaderProgramHandle(),
            _lightingShaderUniformLocations.alpha,
            1,
            &_alpha
    );

    for(int i=0;i<_pointLights.size();i++) {
        glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_pointLightUniformLocations.positionLocs[i],1,&_pointLights[i].position[0]);
        glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_pointLightUniformLocations.colorLocs[i],1,&_pointLights[i].color[0]);
        glProgramUniform1fv(_lightingShaderProgram->getShaderProgramHandle(),_pointLightUniformLocations.linearAttenLocs[i],1,&_pointLights[i].linAtten);
        glProgramUniform1fv(_lightingShaderProgram->getShaderProgramHandle(),_pointLightUniformLocations.quadAttenLocs[i],1,&_pointLights[i].quadAtten);
        glProgramUniform1fv(_lightingShaderProgram->getShaderProgramHandle(),_pointLightUniformLocations.expAttenLocs[i],1,&_pointLights[i].expAtten);
    }

    for(int i=0;i<_dirLights.size();i++) {
        glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_dirLightUniformLocations.directionLocs[i],1,&_dirLights[i].direction[0]);
        glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_dirLightUniformLocations.colorLocs[i],1,&_dirLights[i].color[0]);
        glProgramUniform1fv(_lightingShaderProgram->getShaderProgramHandle(),_dirLightUniformLocations.intensityLocs[i],1,&_dirLights[i].intensity);
    }

    for(int i=0;i<_spotLights.size();i++) {
        glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_spotLightUniformLocations.positionLocs[i],1,&_spotLights[i].position[0]);
        glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_spotLightUniformLocations.dirLocs[i],1,&_spotLights[i].direction[0]);
        glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_spotLightUniformLocations.colorLocs[i],1,&_spotLights[i].color[0]);
        glProgramUniform1fv(_lightingShaderProgram->getShaderProgramHandle(),_spotLightUniformLocations.angleLocs[i],1,&_spotLights[i].angle);
        glProgramUniform1fv(_lightingShaderProgram->getShaderProgramHandle(),_spotLightUniformLocations.linearAttenLocs[i],1,&_spotLights[i].linAtten);
        glProgramUniform1fv(_lightingShaderProgram->getShaderProgramHandle(),_spotLightUniformLocations.quadAttenLocs[i],1,&_spotLights[i].quadAtten);
        glProgramUniform1fv(_lightingShaderProgram->getShaderProgramHandle(),_spotLightUniformLocations.expAttenLocs[i],1,&_spotLights[i].expAtten);

    }
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

}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void mpEngine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const {
    pArcballCam->setLookAtPoint(_currentPlayer->getPosition());

    // Get the player's look-at point (assuming getLookAtPoint() returns the look-at point)
    glm::vec3 lookAtPoint = _currentPlayer->getPosition();

    float yOffset = 0.2f; // moves the fp_cam a little above the center of the player
    glm::vec3 offset(0.0f, yOffset, 0.0f);
    fpCam->setPosition(_currentPlayer->getPosition() + offset);

    pArcballCam->recomputeOrientation();

    // use our lighting shader program
    _lightingShaderProgram->useProgram();

    glm::vec3 lookAtDir;
    if(_currentCam == 1){
        lookAtDir = glm::normalize(pArcballCam->getPosition() - pArcballCam->getLookAtPoint());
    }else if(_currentCam == 2){
        lookAtDir = glm::normalize(pFreeCam->getPosition() - pFreeCam->getLookAtPoint());
    }

    glProgramUniform3fv(_lightingShaderProgram -> getShaderProgramHandle(), _lightingShaderUniformLocations.lookAtDir, 1, &lookAtDir[0]);

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

    for( const MushroomData& currentMushroomData : _mushrooms ) {
        _computeAndSendMatrixUniforms(currentMushroomData.modelMatrix, viewMtx, projMtx);

        _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.materialColor, currentMushroomData.color);

        CSCI441::drawSolidCone(1.0, 2.0, 3, 3);
    }

    for( const MushroomData& currentMushroomData : _mushrooms ) {

//        CSCI441::drawSolidSphere(1.0, 12, 12);
        // Get the existing model matrix and move the sphere up
        glm::mat4 translatedModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 2.5f, 0.0f)) * currentMushroomData.modelMatrix;

        _computeAndSendMatrixUniforms(translatedModelMatrix, viewMtx, projMtx);
        _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.materialColor, currentMushroomData.color);
        CSCI441::drawSolidSphere(1.0, 10, 10);
    }

    //// BEGIN DRAWING THE SKIFF ////
    glm::mat4 modelMtx(1.0f);

    // draw our _players
    for(Player* player: _players){
        player->drawPlayer(modelMtx, viewMtx, projMtx);
    }
//    _currentPlayer->drawPlayer(modelMtx, viewMtx, projMtx);


    //// END DRAWING THE SKIFF ////
}

void mpEngine::_updateScene() {
    _currentPlayer = _players[_currentPlayerIdx];

    GLfloat skiffMovementSpeed = 0.1f;
    // turn right
    if( _keys[GLFW_KEY_D] || _keys[GLFW_KEY_RIGHT] ) {
        GLfloat currAngle = _currentPlayer->getAngle();
        currAngle -= _cameraSpeed.y;
        _currentPlayer->setAngle(currAngle);
        pArcballCam->rotate(_cameraSpeed.y, 0.0f);
        pArcballCam->recomputeOrientation();
//        pOverheadCam->rotate(_cameraSpeed.y, 0.0f);
//        pOverheadCam->recomputeOrientation();
//        _currentPlayer->setPosition(pArcballCam->getLookAtPoint());
    }
    // turn left
    if( _keys[GLFW_KEY_A] || _keys[GLFW_KEY_LEFT] ) {
        GLfloat currAngle = _currentPlayer->getAngle();
        currAngle += _cameraSpeed.y;
        _currentPlayer->setAngle(currAngle);
        pArcballCam->rotate(-_cameraSpeed.y, 0.0f);
        pArcballCam->recomputeOrientation();
//        pOverheadCam->rotate(-_cameraSpeed.y, 0.0f);
//        pOverheadCam->recomputeOrientation();
//        _currentPlayer->setPosition(pArcballCam->getLookAtPoint());
    }
    // go forward
    if( _keys[GLFW_KEY_W] || _keys[GLFW_KEY_UP] ) {
        glm::vec3 currPoint = _currentPlayer->getPosition();
        glm::vec3 projectedPoint = currPoint;
        projectedPoint.z -= skiffMovementSpeed * cos(_currentPlayer->getAngle());
        projectedPoint.x -= skiffMovementSpeed * sin(_currentPlayer->getAngle());
        // ensure skiff stays in ground plane
        if (projectedPoint.z > bottomEdge && projectedPoint.z < topEdge && projectedPoint.x < rightEdge && projectedPoint.x > leftEdge) {
            _currentPlayer->setPosition(projectedPoint);
            pArcballCam->setLookAtPoint(_currentPlayer->getPosition());
            pArcballCam->recomputeOrientation();
//            headCam->setLookAtPoint(projectedPoint);
//            ppOverOverheadCam->recomputeOrientation();
        }
    }
    // go backwards
    if( _keys[GLFW_KEY_S] || _keys[GLFW_KEY_DOWN] ) {
        glm::vec3 currPoint = _currentPlayer->getPosition();
        glm::vec3 projectedPoint = currPoint;
        projectedPoint.z += skiffMovementSpeed * cos(_currentPlayer->getAngle());
        projectedPoint.x += skiffMovementSpeed * sin(_currentPlayer->getAngle());
        // ensure skiff stays in ground plane
        if (projectedPoint.z > bottomEdge && projectedPoint.z < topEdge && projectedPoint.x < rightEdge && projectedPoint.x > leftEdge) {
            _currentPlayer->setPosition(projectedPoint);
            pArcballCam->setLookAtPoint(_currentPlayer->getPosition());
            pArcballCam->recomputeOrientation();
//            pOverheadCam->setLookAtPoint(projectedPoint);
//            pOverheadCam->recomputeOrientation();
        }
    }
    if(_currentCam == 2){
        float currTheta = pFreeCam->getTheta();
        float currPhi = pFreeCam->getPhi();
        glm::vec3 currPos = pFreeCam->getPosition();

        if(_keys[GLFW_KEY_I]){
            currPhi += _cameraSpeed.y;
        }
        if(_keys[GLFW_KEY_K]){
            currPhi -= _cameraSpeed.y;
        }
        if(_keys[GLFW_KEY_J]){
            currTheta -= _cameraSpeed.y;
        }
        if(_keys[GLFW_KEY_L]){
            currTheta += _cameraSpeed.y;
        }
        if(_keys[GLFW_KEY_SPACE]){
            if(_keys[GLFW_KEY_LEFT_SHIFT] || _keys[GLFW_KEY_RIGHT_SHIFT]){
                pFreeCam->moveBackward(_cameraSpeed.x/1.5f);
            }
            else{
                pFreeCam->moveForward(_cameraSpeed.x/1.5f);
            }
        }
        pFreeCam->setTheta(currTheta);
        pFreeCam->setPhi(currPhi);
        pFreeCam->recomputeOrientation();
    }


    //Code to rotate the look direction of the FP cam
    if (_currentCam == 1) {
        // Update yaw based on 'A' and 'D' keys
        if (_keys[GLFW_KEY_A] && !_keys[GLFW_KEY_D]) {
            GLfloat currAngle = _currentPlayer->getAngle();
            currAngle += _cameraSpeed.x;
            // Turn left by decreasing yaw
            //TODO: fix the hard encoding of the FP yaw adjustment
            fpCam->setYaw(fpCam->getYaw() - 1.13);
            //fpCam->setYaw(currAngle);
        } else if (_keys[GLFW_KEY_D] && !_keys[GLFW_KEY_A]) {
            // Turn right by increasing yaw
            //TODO: fix the hard encoding of the FP yaw adjustment
            fpCam->setYaw(fpCam->getYaw() + 1.13);
            //fpCam->setYaw(_currentPlayer->getAngle());
        }

        // Rest of your code...

        // Update view matrix based on the camera's new orientation
        //viewMatrix = fpCam->getViewMatrix();
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
        //ArcBall Camera
        if(_currentCam==1){
            _renderScene(pArcballCam->getViewMatrix(), pArcballCam->getProjectionMatrix());
            //Picture in Picture first Person Camera
            if(_fpCamShown){
                glClear(GL_DEPTH_BUFFER_BIT);
                glViewport(framebufferWidth * 0.75f, framebufferHeight * 0.75f, framebufferWidth * 0.25f, framebufferHeight * 0.25f);
                _renderScene(fpCam->getViewMatrix(), fpCam->getProjectionMatrix());

            }
        }
        //Sky Camera
        else if(_currentCam == 2){
            _renderScene(pFreeCam->getViewMatrix(), pFreeCam->getProjectionMatrix());
        }

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
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.modelMtx, modelMtx);

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