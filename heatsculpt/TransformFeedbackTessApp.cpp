//
//  TransformFeedbackTessApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 20.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//
#include <algorithm>
#include "TransformFeedbackTessApp.h"
#include "ColorUtils.h"
#include "MeshUtils.h"
#include "Debug.h"
TransformFeedbackTessApp::TransformFeedbackTessApp(const std::string& window_title, bool fullscreen):
App::App(window_title, fullscreen){
}

TransformFeedbackTessApp::~TransformFeedbackTessApp(){
    delete mesh;
    delete transformFeedbackShader;
//    delete tesselationShader;

//    glDeleteBuffers(1, &tfSourceVBO);
//    glDeleteBuffers(1, &tfDestinationVBO);
}


bool TransformFeedbackTessApp::InitMesh(){
    
    
    this->mesh = new TransformFeedbackMesh();
    
    vector<vec3> vertices;
    vector<vec3> colors;
    vector<GLuint> indices;
    
//    vertices.push_back(vec3( 0.0f,  0.5f, 0.0f));
//    vertices.push_back(vec3(-0.5f, -0.5f, 0.0f));
//    vertices.push_back(vec3( 0.5f, -0.5f, 0.0f));
//    
//    indices.push_back(0);
//    indices.push_back(1);
//    indices.push_back(2);
    
    createIcosahedron(vertices, indices);
    
    
    generateSingleColor((unsigned int)vertices.size(), colors, vec3(1.0f, 1.0f, 1.0f));
    

    

    // position:
    {
        Attribute positionAttrib;
        positionAttrib.name = "position";
        positionAttrib.num_of_components = 3;
        positionAttrib.data_type = GL_FLOAT;
        positionAttrib.buffer_type = GL_ARRAY_BUFFER;
        
        
        Attribute destPositionAttrib;
        std::memcpy(&destPositionAttrib, &positionAttrib, sizeof (Attribute));
        
        mesh->addVBO(vertices, positionAttrib, mesh->getVAO());
        mesh->addVBO(vertices, destPositionAttrib, mesh->getDestVAO());
        
        transformFeedbackShader->use();
        positionAttrib.id = transformFeedbackShader->addAttribute(positionAttrib.name);
        destPositionAttrib.id = positionAttrib.id;
        
        mesh->setupAttribute(mesh->getVAO(), positionAttrib);
        mesh->setupAttribute(mesh->getDestVAO(), destPositionAttrib);
        transformFeedbackShader->disable();
        

        
        mesh->attributes.push_back(positionAttrib);
        mesh->dest_attributes.push_back(destPositionAttrib);
    }
    
    // color:
    {
        Attribute colorAttrib;
        colorAttrib.name = "color";
        colorAttrib.num_of_components = 3;
        colorAttrib.data_type = GL_FLOAT;
        colorAttrib.buffer_type = GL_ARRAY_BUFFER;
        
        
        
        Attribute destColorAttrib;
        std::memcpy(&destColorAttrib, &colorAttrib, sizeof (Attribute));
        
        mesh->addVBO(vertices, colorAttrib, mesh->getVAO());
        mesh->addVBO(vertices, destColorAttrib, mesh->getDestVAO());
        
        transformFeedbackShader->use();
        colorAttrib.id = transformFeedbackShader->addAttribute(colorAttrib.name);
        destColorAttrib.id = colorAttrib.id;
        
        mesh->setupAttribute(mesh->getVAO(), colorAttrib);
        mesh->setupAttribute(mesh->getDestVAO(), destColorAttrib);
        transformFeedbackShader->disable();
        
        
        
        mesh->attributes.push_back(colorAttrib);
        mesh->dest_attributes.push_back(destColorAttrib);
    }
    
    
    
    // indices:
    {
        mesh->addIndices(indices);
    }
    
    
    
    // uniforms:
    {
//        drawShader->use();
//        GLuint model = drawShader->addUniform("model");
//        glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix));
//        GLuint view = drawShader->addUniform("view");
//        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera.view));
//        GLuint projection = drawShader->addUniform("projection");
//        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
//        drawShader->disable();
        
    }

    
    return true;
}


bool TransformFeedbackTessApp::InitTransformFeedbackShader(){
    this->transformFeedbackShader = new ShaderProgram();
    
    Shader vertexShader(GL_VERTEX_SHADER);
    vertexShader.loadFromFile("shaders/tfsimple.vert");
    vertexShader.compile();
    
    Shader fragmentShader(GL_FRAGMENT_SHADER);
    fragmentShader.loadFromFile("shaders/tfsimple.frag");
    fragmentShader.compile();
    
    this->transformFeedbackShader->attachShader(vertexShader);
    this->transformFeedbackShader->attachShader(fragmentShader);
    
    
    const char *varyings[] = {"outposition"};
    glTransformFeedbackVaryings(transformFeedbackShader->id(), 1, varyings, GL_INTERLEAVED_ATTRIBS);
    
    bool res = this->transformFeedbackShader->linkProgram();
    if (!res) {
        return false;
    }
    
    
//    glGenVertexArrays(1, &tfSourceVAO);
//    glGenVertexArrays(1, &tfDestinationVAO);
//    glGenBuffers(1, &tfSourceVBO);
//    glGenBuffers(1, &tfDestinationVBO);
    
    
    return true;
}
bool TransformFeedbackTessApp::InitTesselationShader(){
    
    return true;
}


bool TransformFeedbackTessApp::Init(){
    bool res = App::Init();
    if (!res) {
        return res;
    }

    
    
    res = InitTransformFeedbackShader();
    if (!res) {
        return res;
    }
    
    res = InitTesselationShader();
    if (!res) {
        return res;
    }
    
    
    res = InitMesh();
    if (!res) {
        return res;
    }
    
    return true;
}
void TransformFeedbackTessApp::Update(){
    App::Update();
    
//    mesh->Update();
    transformFeedbackShader->use();
    
//    GLuint m = drawShader->uniform("model");
//    glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix));
//
//    GLuint v = drawShader->uniform("view");
//    glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(camera.view));
//    
//    GLuint p = drawShader->uniform("projection");
//    glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(camera.projection));
    
    
    transformFeedbackShader->disable();

    
}
void TransformFeedbackTessApp::Render(){
    App::Render();
    
    this->transformFeedbackShader->use();
    this->mesh->Draw(GL_TRIANGLES);
    this->transformFeedbackShader->disable();
    
}