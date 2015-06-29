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
    positionAttrib.data_type = GL_FLOAT;
    positionAttrib.buffer_type = GL_ARRAY_BUFFER;
    attributes.push_back(positionAttrib);
    
    Attribute colorAttrib;
    colorAttrib.name = "Color";
    colorAttrib.num_of_components = 3;
    colorAttrib.data_type = GL_FLOAT;
    colorAttrib.buffer_type = GL_ARRAY_BUFFER;
    attributes.push_back(colorAttrib);
    
    
    //createTetraedron(vertices, indices);
    createIcosphere(vertices, indices);
    
    mesh = new Mesh(shaderProgram, attributes,  vertices, indices);
//    mesh = new Mesh(shaderProgram, vertices, indices);
    
    shaderProgram->use();
    GLuint model = shaderProgram->addUniform("model");
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix));
    GLuint view = shaderProgram->addUniform("view");
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera.view));
    GLuint projection = shaderProgram->addUniform("projection");
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
    shaderProgram->disable();
        

    
    
    return res;
}

void MeshTestApp::Update(){
    App::Update();

    
    mesh->Update();
    
    shaderProgram->use();
    
            GLuint m = shaderProgram->uniform("model");
            glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix));
    
            GLuint v = shaderProgram->uniform("view");
            glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(camera.view));
    
            GLuint p = shaderProgram->uniform("projection");
            glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(camera.projection));

    
    shaderProgram->disable();
    
}

void MeshTestApp::Render(){
    App::Render();
    
    mesh->Draw();
}