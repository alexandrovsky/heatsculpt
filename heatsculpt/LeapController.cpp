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
hands(NULL), modelMatrix(1),fingerVertices(num_of_finger_points){
}
LeapController::~LeapController(){
    delete [] hands;
    delete handsDrawShader;
}

void LeapController::Init(){
    controller.addListener(*this);
    
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
    
    
    hands = new Mesh[2];
    
//    fingerVertices(3 * 5);
    
    
    for(int i = 0; i< num_of_finger_points; i++) {
        
        // initial position
        fingerVertices[i] = glm::normalize( glm::vec3(0.5f-float(std::rand())/RAND_MAX,
                                                      0.5f-float(std::rand())/RAND_MAX,
                                                      0.5f-float(std::rand())/RAND_MAX
                                                     ));
        
        //fingerVertices[i] += vec3(10, 0, 0);
        fingerVertices[i] *= 25.0f;
    }
    
    
    vector<GLuint> fingerIndices(3 * 5);
    vector<vec3> fingerColors(3 * 5);
    
    // position
    {
        
        positionAttrib.name = "position";
        positionAttrib.num_of_components = 3;
        positionAttrib.data_type = GL_FLOAT;
        positionAttrib.buffer_type = GL_ARRAY_BUFFER;
        
        generateSingleColor((unsigned int)fingerColors.size(), fingerColors, vec3(1.0));
        
        hands[0].addVBO(fingerVertices, positionAttrib);
        hands[1].addVBO(fingerVertices, positionAttrib);
        
        
        handsDrawShader->use();
        positionAttrib.id = handsDrawShader->addAttribute(positionAttrib.name);
        glEnableVertexAttribArray(positionAttrib.id);
        glVertexAttribPointer(positionAttrib.id, positionAttrib.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
        handsDrawShader->disable();
        
        hands[0].attributes.push_back(positionAttrib);
        hands[1].attributes.push_back(positionAttrib);
    }
    
    
    // color:
    {
        
        colorAttrib.name = "color";
        colorAttrib.num_of_components = 3;
        colorAttrib.data_type = GL_FLOAT;
        colorAttrib.buffer_type = GL_ARRAY_BUFFER;
        
        
        
        hands[0].addVBO(fingerColors, colorAttrib);
        hands[1].addVBO(fingerColors, colorAttrib);
        handsDrawShader->use();
        colorAttrib.id = handsDrawShader->addAttribute(colorAttrib.name);
        glEnableVertexAttribArray(colorAttrib.id);
        glVertexAttribPointer(colorAttrib.id, colorAttrib.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
        handsDrawShader->disable();
        
        hands[0].attributes.push_back(colorAttrib);
        hands[1].attributes.push_back(colorAttrib);
        
        
        // indices:
        {
            for (int i = 0; i < num_of_finger_points; i++) {
                fingerIndices[i] = i;
            }
            
            hands[0].addIndices(fingerIndices);
            hands[1].addIndices(fingerIndices);
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
    controller.removeListener(*this);

}


void LeapController::Render(mat4 view, mat4 projection){
    handsDrawShader->use();

    
    GLuint m = handsDrawShader->uniform("model");
    glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    
    GLuint v = handsDrawShader->uniform("view");
    glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(view));
    
    GLuint p = handsDrawShader->uniform("projection");
    glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(projection));
    
    
    HandList leapHands = frame.hands();
    for (int i = 0; i < leapHands.count(); i++) {
        
        FingerList leapFingers = leapHands[i].fingers();
        
        
        
        if(leapHands[i].isLeft()){
            
            for (int j = 0; j < leapFingers.count(); j++) {
                Leap::Vector tip = leapFingers[j].tipPosition();
                fingerVertices[j] =  0.2f * vec3(tip.x, tip.y, tip.z);
            }
            
            hands[0].setBufferData(fingerVertices, positionAttrib);
        }else{
            for (int j = 0; j < leapFingers.count(); j++) {
                Leap::Vector tip = leapFingers[j].tipPosition();
                fingerVertices[j+num_of_finger_points/2-1] =  0.2f * vec3(tip.x, tip.y, tip.z);
            }
            hands[1].setBufferData(fingerVertices, positionAttrib);
        }
    }
    
    
    hands[0].Draw(GL_POINTS);
    hands[1].Draw(GL_POINTS);
    
    

    

    
    
    handsDrawShader->disable();
    
}

void LeapController::onInit(const Controller& controller) {
    std::cout << "Initialized" << std::endl;
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
    frame = controller.frame();
    std::cout << "Frame id: " << frame.id()
    << ", timestamp: " << frame.timestamp()
    << ", hands: " << frame.hands().count()
    << ", extended fingers: " << frame.fingers().extended().count()
    << ", tools: " << frame.tools().count()
    << ", gestures: " << frame.gestures().count() << std::endl;
    
    HandList hands = frame.hands();
    for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
        // Get the first hand
        const Hand hand = *hl;
        std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
        std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
        << ", palm position: " << hand.palmPosition() << std::endl;
        // Get the hand's normal vector and direction
        const Vector normal = hand.palmNormal();
        const Vector direction = hand.direction();
        
        // Calculate the hand's pitch, roll, and yaw angles
        std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
        << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
        << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;
        
        // Get the Arm bone
        Arm arm = hand.arm();
        std::cout << std::string(2, ' ') <<  "Arm direction: " << arm.direction()
        << " wrist position: " << arm.wristPosition()
        << " elbow position: " << arm.elbowPosition() << std::endl;
        
        // Get fingers
        const FingerList fingers = hand.fingers();
        for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
            const Finger finger = *fl;
            std::cout << std::string(4, ' ') <<  fingerNames[finger.type()]
            << " finger, id: " << finger.id()
            << ", length: " << finger.length()
            << "mm, width: " << finger.width() << std::endl;
            
            // Get finger bones
            for (int b = 0; b < 4; ++b) {
                Bone::Type boneType = static_cast<Bone::Type>(b);
                Bone bone = finger.bone(boneType);
                std::cout << std::string(6, ' ') <<  boneNames[boneType]
                << " bone, start: " << bone.prevJoint()
                << ", end: " << bone.nextJoint()
                << ", direction: " << bone.direction() << std::endl;
            }
        }
    }
    
    // Get tools
    const ToolList tools = frame.tools();
    for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
        const Tool tool = *tl;
        std::cout << std::string(2, ' ') <<  "Tool, id: " << tool.id()
        << ", position: " << tool.tipPosition()
        << ", direction: " << tool.direction() << std::endl;
    }
    
    // Get gestures
    const GestureList gestures = frame.gestures();
    for (int g = 0; g < gestures.count(); ++g) {
        Gesture gesture = gestures[g];
        
        switch (gesture.type()) {
            case Gesture::TYPE_CIRCLE:
            {
                CircleGesture circle = gesture;
                std::string clockwiseness;
                
                if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
                    clockwiseness = "clockwise";
                } else {
                    clockwiseness = "counterclockwise";
                }
                
                // Calculate angle swept since last frame
                float sweptAngle = 0;
                if (circle.state() != Gesture::STATE_START) {
                    CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
                    sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
                }
                std::cout << std::string(2, ' ')
                << "Circle id: " << gesture.id()
                << ", state: " << stateNames[gesture.state()]
                << ", progress: " << circle.progress()
                << ", radius: " << circle.radius()
                << ", angle " << sweptAngle * RAD_TO_DEG
                <<  ", " << clockwiseness << std::endl;
                break;
            }
            case Gesture::TYPE_SWIPE:
            {
                SwipeGesture swipe = gesture;
                std::cout << std::string(2, ' ')
                << "Swipe id: " << gesture.id()
                << ", state: " << stateNames[gesture.state()]
                << ", direction: " << swipe.direction()
                << ", speed: " << swipe.speed() << std::endl;
                break;
            }
            case Gesture::TYPE_KEY_TAP:
            {
                KeyTapGesture tap = gesture;
                std::cout << std::string(2, ' ')
                << "Key Tap id: " << gesture.id()
                << ", state: " << stateNames[gesture.state()]
                << ", position: " << tap.position()
                << ", direction: " << tap.direction()<< std::endl;
                break;
            }
            case Gesture::TYPE_SCREEN_TAP:
            {
                ScreenTapGesture screentap = gesture;
                std::cout << std::string(2, ' ')
                << "Screen Tap id: " << gesture.id()
                << ", state: " << stateNames[gesture.state()]
                << ", position: " << screentap.position()
                << ", direction: " << screentap.direction()<< std::endl;
                break;
            }
            default:
                std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
                break;
        }
    }
    
    if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
        std::cout << std::endl;
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

//int main(int argc, char** argv) {
//    // Create a sample listener and controller
//    LeapController listener;
//    Controller controller;
//    
//    // Have the sample listener receive events from the controller
//    controller.addListener(listener);
//    
//    if (argc > 1 && strcmp(argv[1], "--bg") == 0)
//        controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
//    
//    // Keep this process running until Enter is pressed
//    std::cout << "Press Enter to quit..." << std::endl;
//    std::cin.get();
//    
//    // Remove the sample listener when done
//    controller.removeListener(listener);
//    
//    return 0;
//}
