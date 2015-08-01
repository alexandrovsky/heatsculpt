//
//  LeapController.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 01.08.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "LeapController.h"
#include "ColorUtils.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

LeapController::LeapController():Listener(),
drawHands(NULL), modelMatrix(1),
leftFingerVertices(num_of_finger_points),
rightFingerVertices(num_of_finger_points),
fingerIndices(num_of_finger_points),
fingerColors(num_of_finger_points),
initialized(false){
}
LeapController::~LeapController(){
    delete [] drawHands;
    drawHands = NULL;
    delete handsDrawShader;
    handsDrawShader = NULL;
}

void LeapController::initDrawing(){
    
    // drawHandsShader
    {
        Shader vertexShader(GL_VERTEX_SHADER);
        vertexShader.loadFromFile("shaders/simple.vert");
        vertexShader.compile();
        
        Shader fragmentShader(GL_FRAGMENT_SHADER);
        fragmentShader.loadFromFile("shaders/simple.frag");
        fragmentShader.compile();
        
//        Shader geometryShader(GL_GEOMETRY_SHADER);
//        geometryShader.loadFromFile("shaders/tess.geom");
//        geometryShader.compile();
//        
//        Shader tesselationControlShader(GL_TESS_CONTROL_SHADER);
//        tesselationControlShader.loadFromFile("shaders/tess.tcs");
//        tesselationControlShader.compile();
//        
//        Shader tesselationEvaluationShader(GL_TESS_EVALUATION_SHADER);
//        tesselationEvaluationShader.loadFromFile("shaders/tess.tes");
//        tesselationEvaluationShader.compile();
        
        handsDrawShader = new ShaderProgram();
        handsDrawShader->attachShader(vertexShader);
        handsDrawShader->attachShader(fragmentShader);
//        handsDrawShader->attachShader(geometryShader);
//        handsDrawShader->attachShader(tesselationControlShader);
//        handsDrawShader->attachShader(tesselationEvaluationShader);
        handsDrawShader->linkProgram();

    }
    
    
    drawHands = new Mesh[2];
    
    
    for(int i = 0; i< num_of_finger_points; i++) {
        
        // initial position
        leftFingerVertices[i] = glm::normalize( glm::vec3(1.5f-float(std::rand())/RAND_MAX,
                                                      0.5f-float(std::rand())/RAND_MAX,
                                                      0.5f-float(std::rand())/RAND_MAX
                                                     ));
        leftFingerVertices[i] *= 25.0f;
        
        rightFingerVertices[i] = glm::normalize( glm::vec3(1.5f+float(std::rand())/RAND_MAX,
                                                           0.5f-float(std::rand())/RAND_MAX,
                                                           0.5f-float(std::rand())/RAND_MAX
                                                           ));
        
        rightFingerVertices[i] *= 25.0f;
    }
    
    
    // position
    {
        
        positionAttrib.name = "position";
        positionAttrib.num_of_components = 3;
        positionAttrib.data_type = GL_FLOAT;
        positionAttrib.buffer_type = GL_ARRAY_BUFFER;
        
        generateSingleColor((unsigned int)fingerColors.size(), fingerColors, vec3(1.0));
        
        drawHands[0].addVBO(leftFingerVertices, positionAttrib);
        drawHands[1].addVBO(rightFingerVertices, positionAttrib);
        
        
        handsDrawShader->use();
        positionAttrib.id = handsDrawShader->addAttribute(positionAttrib.name);
        glEnableVertexAttribArray(positionAttrib.id);
        glVertexAttribPointer(positionAttrib.id, positionAttrib.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
        handsDrawShader->disable();
        
        drawHands[0].attributes.push_back(positionAttrib);
        drawHands[1].attributes.push_back(positionAttrib);
    }
    
    
    // color:
    {
        
        colorAttrib.name = "color";
        colorAttrib.num_of_components = 3;
        colorAttrib.data_type = GL_FLOAT;
        colorAttrib.buffer_type = GL_ARRAY_BUFFER;
        
        
        
        drawHands[0].addVBO(fingerColors, colorAttrib);
        drawHands[1].addVBO(fingerColors, colorAttrib);
        handsDrawShader->use();
        colorAttrib.id = handsDrawShader->addAttribute(colorAttrib.name);
        glEnableVertexAttribArray(colorAttrib.id);
        glVertexAttribPointer(colorAttrib.id, colorAttrib.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
        handsDrawShader->disable();
        
        drawHands[0].attributes.push_back(colorAttrib);
        drawHands[1].attributes.push_back(colorAttrib);
        
        
        // indices:
        {
            for (int i = 0; i < num_of_finger_points; i++) {
                fingerIndices[i] = i;
            }
            
            drawHands[0].addIndices(fingerIndices);
            drawHands[1].addIndices(fingerIndices);
        }
        
        
        
        // uniforms:
        {
            handsDrawShader->use();
            handsDrawShader->addUniform("model");
            handsDrawShader->addUniform("view");
            handsDrawShader->addUniform("projection");
            handsDrawShader->disable();
            
        }
    }
}

void LeapController::Destroy(){

}


void LeapController::Render(mat4 view, mat4 projection){
    
    if (!initialized) {
        return;
    }
    
    
    for (int i = 0; i < 2; i ++) {
        handsDrawShader->use();
        
        
        GLuint m = handsDrawShader->uniform("model");
        glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        
        GLuint v = handsDrawShader->uniform("view");
        glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(view));
        
        GLuint p = handsDrawShader->uniform("projection");
        glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(projection));
        
        
        if (i %2 == 0) {
            
        }else{
            drawHands[i].setBufferData(rightFingerVertices, positionAttrib);
        }
        
        
        drawHands[i].Draw(GL_POINTS);
        
        handsDrawShader->disable();
    }
    
    
    
}

void LeapController::onInit(const Controller& controller) {
    std::cout << "Initialized" << std::endl;
    initDrawing();
    initialized = true;
}

void LeapController::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
    controller.enableGesture(Gesture::TYPE_CIRCLE);
    controller.enableGesture(Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Gesture::TYPE_SWIPE);
}

void LeapController::onDisconnect(const Controller& controller) {
    // Note: not dispatched when running in a debugger.
    std::cout << "Disconnected" << std::endl;
}

void LeapController::onExit(const Controller& controller) {
    std::cout << "Exited" << std::endl;
}

void LeapController::onFrame(const Controller& controller) {
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();
    
    
    HandList hands = frame.hands();
    for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
        // Get the first hand
        const Hand hand = *hl;
        
        
        // Get fingers
        const FingerList fingers = hand.fingers();
        for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
            const Finger finger = *fl;
            

            
            // Get finger bones
            for (int b = 0; b < 4; ++b) {
                Bone::Type boneType = static_cast<Bone::Type>(b);
                Bone bone = finger.bone(boneType);
                
                Leap::Vector joint = finger.bone(boneType).prevJoint();
                vec3 p = 0.2f * vec3(joint.x, joint.y, joint.z);
                
                uint idx = finger.type()*4+b;
                if(hand.isLeft()){
                    leftFingerVertices[idx] = p;
                }else{
                    rightFingerVertices[idx] = p;
                }
            }
        }

    }
    
    
}

void LeapController::onFocusGained(const Controller& controller) {
    std::cout << "Focus Gained" << std::endl;
}

void LeapController::onFocusLost(const Controller& controller) {
    std::cout << "Focus Lost" << std::endl;
}

void LeapController::onDeviceChange(const Controller& controller) {
    std::cout << "Device Changed" << std::endl;
    const DeviceList devices = controller.devices();
    
    for (int i = 0; i < devices.count(); ++i) {
        std::cout << "id: " << devices[i].toString() << std::endl;
        std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
    }
}

void LeapController::onServiceConnect(const Controller& controller) {
    std::cout << "Service Connected" << std::endl;
}

void LeapController::onServiceDisconnect(const Controller& controller) {
    std::cout << "Service Disconnected" << std::endl;
}
