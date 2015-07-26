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
    
    
    TransformFeedbackAttribute* positionAttrib = new TransformFeedbackAttribute();
    positionAttrib->name = "inposition";
    positionAttrib->num_of_components = 3;
    positionAttrib->data_type = GL_FLOAT;
    positionAttrib->buffer_type = GL_ARRAY_BUFFER;
    positionAttrib->draw_type = GL_DYNAMIC_DRAW;
    
    positionAttrib->id = transformFeedback->shaderProgram->addAttribute(positionAttrib->name);
    transformFeedback->varyingAttributs.push_back(positionAttrib);
    
    
    vector<glm::vec3> vertices;
    for (int i = 0; i < 100000; i++) {
        float x = 0.5f - static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float y = 0.5f - static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float z = 0.5f - static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        vertices.push_back(glm::normalize(glm::vec3(x, y, z)) );
    }
    
    transformFeedback->SetDrawCount((GLsizei)vertices.size());
    transformFeedback->AddBuffer(vertices, positionAttrib);
    
    //transformFeedback->InitTransformFeedback();

    
    return true;
}
void TransformFeedbackMeshApp::Update(){
    transformFeedback->Update();
}
void TransformFeedbackMeshApp::Render(){
    App::Render();
    transformFeedback->Draw();
}