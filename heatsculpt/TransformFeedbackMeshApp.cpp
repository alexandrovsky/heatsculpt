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
    delete transformFeedback;
}



bool TransformFeedbackMeshApp::Init(){
    bool res = App::Init();
    
    if (!res) {
        return false;
    }
    
    transformFeedback = new TransformFeedback();
    
    vector<const char *> varyings;
    varyings.push_back("outposition");
    
    vector<pair<string, GLenum>> shaderSources;
    shaderSources.push_back(pair<string, GLenum>("shaders/tfSimple.vert", GL_VERTEX_SHADER));
    shaderSources.push_back(pair<string, GLenum>("shaders/tfSimple.frag", GL_FRAGMENT_SHADER));
    
    transformFeedback->InitShader(shaderSources, varyings);
    transformFeedback->InitTransformFeedback();

    
    return true;
}
void TransformFeedbackMeshApp::Update(){
    transformFeedback->Update();
}
void TransformFeedbackMeshApp::Render(){
    App::Render();
    transformFeedback->Draw();
}