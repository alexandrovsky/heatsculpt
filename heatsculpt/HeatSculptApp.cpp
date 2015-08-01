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
:App(window_title, fullscreen),
hands(NULL){
    
    clay = new Clay();
    
}
HeatSculptApp::~HeatSculptApp(){
    leap.Destroy();
    delete clay;
    delete [] hands;
}


bool HeatSculptApp::Init(){
    App::Init();
    
    // we ar blending so no depth testing
//    glDisable(GL_DEPTH_TEST);
    
    // enable blending
//    glEnable(GL_BLEND);
//    //  and set the blend function to result = 1*source + 1*destination
//    glBlendFunc(GL_ONE, GL_ONE);
//    

    camera.SetPosition(glm::vec3(0.0f, 0.0f, -130.0f));
    
    clay->Init();
    leap.Init();
    
    
    hands = new Mesh[2];
    
    
    
    
    Attribute positionAttrib;
    positionAttrib.name = "position";
    positionAttrib.num_of_components = 3;
    positionAttrib.data_type = GL_FLOAT;
    positionAttrib.buffer_type = GL_ARRAY_BUFFER;
    
    vector<vec3> fingerVertices(3 * 5);
    vector<vec3> fingerColors(3 * 5);
    
    hands[0].addVBO(fingerVertices, positionAttrib);
    
    
    return true;
}

void HeatSculptApp::Update(){
    App::Update();
    clay->Update();
}

void HeatSculptApp::Render(){
    App::Render();
    clay->Render(camera.view, camera.projection);

}


void HeatSculptApp::OnKeyUp(GLFWwindow* window, int key, int scancode, int action, int mods){
    App::OnKeyUp(window, key, scancode, action, mods);
    
    switch (key) {
        case GLFW_KEY_X:
            clay->click = 1;
            break;
        default:
            break;
    }
}
