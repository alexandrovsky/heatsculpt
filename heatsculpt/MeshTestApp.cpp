//
//  MeshTestApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 17.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "MeshTestApp.h"



void createTetraedron(vector<vec3>& vertices, vector<GLuint>& indices){
    
    vertices.push_back(glm::vec3(  1.0,  1.0,  1.0));
    vertices.push_back(glm::vec3( -1.0, -1.0,  1.0));
    vertices.push_back(glm::vec3( -1.0,  1.0, -1.0));
    vertices.push_back(glm::vec3(  1.0, -1.0, -1.0));
    
    
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);
    
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(3);
    
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);
    
}

void createIcosphere(vector<vec3>& vertices, vector<GLuint>& indices){
    GLfloat t = (1.0f + sqrtf(5.0)) /2.0f;
    
    // create 12 vertices of a icosahedron
    
    vertices.push_back(vec3(-1,  t,  0));
    vertices.push_back(vec3( 1,  t,  0));
    vertices.push_back(vec3(-1, -t,  0));
    vertices.push_back(vec3( 1, -t,  0));
    
    vertices.push_back(vec3( 0, -1,  t));
    vertices.push_back(vec3( 0,  1,  t));
    vertices.push_back(vec3( 0, -1, -t));
    vertices.push_back(vec3( 0,  1, -t));
    
    vertices.push_back(vec3( t,  0, -1));
    vertices.push_back(vec3( t,  0,  1));
    vertices.push_back(vec3(-t,  0, -1));
    vertices.push_back(vec3(-t,  0,  1));
    
    // create 20 triangles of the icosahedron


    
    // 5 faces around point 0
    indices.push_back(0); indices.push_back(11); indices.push_back(5);
    indices.push_back(0); indices.push_back(5); indices.push_back(1);
    indices.push_back(0); indices.push_back(1); indices.push_back(7);
    indices.push_back(0); indices.push_back(7); indices.push_back(10);
    indices.push_back(0); indices.push_back(10); indices.push_back(11);

    
    // 5 adjacent faces
    indices.push_back(1); indices.push_back(5); indices.push_back(9);
    indices.push_back(5); indices.push_back(11); indices.push_back(4);
    indices.push_back(11); indices.push_back(10); indices.push_back(2);
    indices.push_back(10); indices.push_back(7); indices.push_back(6);
    indices.push_back(7); indices.push_back(1); indices.push_back(8);

    
    // 5 faces around point 3
    indices.push_back(3); indices.push_back(9); indices.push_back(4);
    indices.push_back(3); indices.push_back(4); indices.push_back(2);
    indices.push_back(3); indices.push_back(2); indices.push_back(6);
    indices.push_back(3); indices.push_back(6); indices.push_back(8);
    indices.push_back(3); indices.push_back(8); indices.push_back(9);

    
//    // 5 adjacent faces
    indices.push_back(4); indices.push_back(9); indices.push_back(5);
    indices.push_back(2); indices.push_back(4); indices.push_back(11);
    indices.push_back(6); indices.push_back(2); indices.push_back(10);
    indices.push_back(8); indices.push_back(6); indices.push_back(7);
    indices.push_back(9); indices.push_back(8); indices.push_back(1);

}


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
    
    //createTetraedron(vertices, indices);
    createIcosphere(vertices, indices);
    
    mesh = new Mesh(shaderProgram, vertices, indices);
    
    
    
    return res;
}

void MeshTestApp::Update(){
    App::Update();
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(45.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
    
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