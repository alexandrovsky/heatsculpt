//
//  MeshTestApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 17.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "MeshTestApp.h"
#include "Vertex.h"


MeshTestApp::MeshTestApp(const std::string& window_title, int window_width, int window_height):
App::App(window_title, window_width, window_height)
{
    
}



MeshTestApp::~MeshTestApp()
{
    delete mesh;
}


bool MeshTestApp::Init(){
    bool res = App::Init();
    if (!res) { return res; }
    
    vertexShader = new Shader(GL_VERTEX_SHADER);
    vertexShader->loadFromString(vertexShaderSrc);
    vertexShader->compile();
    
    fragmentShader = new Shader(GL_FRAGMENT_SHADER);
    fragmentShader->loadFromString(fragmentShaderSrc);
    fragmentShader->compile();
    
    
    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader(*vertexShader);
    shaderProgram->attachShader(*fragmentShader);
    shaderProgram->linkProgram();
    shaderProgram->use();
    
    vector<Vertex> vertices;
    
    
    
    
    vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3( 1.0f, -1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3( 0.0f,  1.0f, 0.0f)));

    
    mesh = new Mesh(shaderProgram, vertices);
    
    
    
    return res;
}

void MeshTestApp::Update(){
    App::Update();
    
    glm::mat4 model;
    model = glm::rotate(model, 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    
    mesh->Update();
    
}

void MeshTestApp::Render(){
    App::Render();
    
    mesh->Draw();
}