//
//  TransformFeedbackMeshApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 24.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "TransformFeedbackMeshApp.h"

TransformFeedbackMeshApp::TransformFeedbackMeshApp(const std::string& window_title, bool fullscreen):App::App(window_title, fullscreen){

}


TransformFeedbackMeshApp::~TransformFeedbackMeshApp(){
}



bool TransformFeedbackMeshApp::Init(){
    bool res = App::Init();
    
    if (!res) {
        return false;
    }
    
    transformFeedback.Init();
    
    
    return true;
}
void TransformFeedbackMeshApp::Update(){
    transformFeedback.Update();
}
void TransformFeedbackMeshApp::Render(){
    App::Render();
}