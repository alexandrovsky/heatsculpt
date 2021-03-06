//
//  HeatSculptApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 31.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "HeatSculptApp.h"
#include <stddef.h>


HeatSculptApp::HeatSculptApp(const std::string& window_title, bool fullscreen)
:App(window_title, fullscreen){
    
    clay = new Clay();
    
}
HeatSculptApp::~HeatSculptApp(){
    
}


bool HeatSculptApp::Init(){
    App::Init();
    
    controller.addListener(leap);
    // we ar blending so no depth testing
//    glDisable(GL_DEPTH_TEST);
    
    // enable blending
//    glEnable(GL_BLEND);
//    //  and set the blend function to result = 1*source + 1*destination
//    glBlendFunc(GL_ONE, GL_ONE);
//    

    camera.SetPosition(glm::vec3(0.0f, 20.0f, 130.0f));
    
    clay->Init();
    
    

    
    return true;
}



void HeatSculptApp::Update(){
    App::Update();
    
    Leap::Frame frame = controller.frame();
    
    if (frame.hands().count() > 0) {
        Leap::Vector leapPos = frame.hands()[0].fingers()[0].tipPosition();
        Leap::Vector leapNormal = frame.hands()[0].fingers()[0].direction();
        vec3 pos = leapVector2glmVector(leapPos);
        vec3 normal = leapVector2glmVector(leapNormal);
        
        clay->setInteractionPoint("interaction0", 1, pos, normal);
    }else{
        clay->setInteractionPoint("interaction0", 0, vec3(0), vec3(0));
    }
    
    

    clay->Update();
}

void HeatSculptApp::Render(){
    App::Render();
    clay->Render(camera.view, camera.projection);
    leap.Render(camera.view, camera.projection);
}

void HeatSculptApp::Exit(){
    App::Exit();
    controller.removeListener(leap);
    leap.Destroy();
    delete clay;

}

//void HeatSculptApp::OnKeyUp(GLFWwindow* window, int key, int scancode, int action, int mods){
//    
//    
//    switch (key) {
//        case GLFW_KEY_X:
//            clay->click = 1;
//            break;
//        default:
//            break;
//    }
//}
