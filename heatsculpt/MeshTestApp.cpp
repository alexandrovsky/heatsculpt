//
//  MeshTestApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 17.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "MeshTestApp.h"
#include "MeshUtils.h"



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
    
    vector<vec3> vertices;
    vector<GLuint> indices;
    vector<Attribute>attributes;
    
    Attribute positionAttrib;
    positionAttrib.name = "Position";
    positionAttrib.num_of_components = 3;
    positionAttrib.type = GL_FLOAT;
    positionAttrib.buffertype = GL_ARRAY_BUFFER;
    attributes.push_back(positionAttrib);
    
    Attribute colorAttrib;
    colorAttrib.name = "Color";
    colorAttrib.num_of_components = 3;
    colorAttrib.type = GL_FLOAT;
    colorAttrib.buffertype = GL_ARRAY_BUFFER;
    attributes.push_back(colorAttrib);
    
    
    //createTetraedron(vertices, indices);
    createIcosphere(vertices, indices);
    
    mesh = new Mesh(shaderProgram, attributes,  vertices, indices);
//    mesh = new Mesh(shaderProgram, vertices, indices);
    
    shaderProgram->use();
    GLuint model = shaderProgram->addUniform("model");
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix));
    GLuint view = shaderProgram->addUniform("view");
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(mesh->viewMatrix));
    GLuint projection = shaderProgram->addUniform("projection");
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(mesh->projectionMatrix));
    shaderProgram->disable();
        

    
    
    return res;
}

void MeshTestApp::Update(){
    App::Update();
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(0.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
    
    //model = glm::translate(model, vec3(0.5f, 0.0f, 0.0f));
    
    mesh->modelMatrix = model;
    
    
    
    mesh->viewMatrix = camera.view;
    mesh->projectionMatrix = camera.projection;
    mesh->Update();
    
}

void MeshTestApp::Render(){
    App::Render();
    
    mesh->Draw();
}