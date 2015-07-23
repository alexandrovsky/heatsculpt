//
//  TestApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 15.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "TestApp.h"








TestApp::TestApp(const std::string& window_title):
    App::App(window_title, true)
{

}



TestApp::~TestApp()
{
}


bool TestApp::Init(){
    bool res = App::Init();
    if (!res) { return res; }
    
    vertexShader = new Shader(GL_VERTEX_SHADER);
    vertexShader->loadFromString(vertexShaderSrc);
    vertexShader->compile();
    
    fragmentShader = new Shader(GL_FRAGMENT_SHADER);
    fragmentShader->loadFromString(fragmentShaderSrc);
    fragmentShader->compile();
    
    geometryShader = new Shader(GL_GEOMETRY_SHADER_ARB);
    geometryShader->loadFromString(geometryShaderSrc);
    geometryShader->compile();
    
    
    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader(*vertexShader);
    shaderProgram->attachShader(*fragmentShader);
    shaderProgram->attachShader(*geometryShader);
    shaderProgram->linkProgram();
    shaderProgram->use();
    
    
    float points[] = {
        //  Coordinates  Color             Sides
        -0.45f,  0.45f, 1.0f, 0.0f, 0.0f,  4.0f,
        0.45f,  0.45f, 0.0f, 1.0f, 0.0f,  8.0f,
        0.45f, -0.45f, 0.0f, 0.0f, 1.0f, 16.0f,
        -0.45f, -0.45f, 1.0f, 1.0f, 0.0f, 32.0f
    };
    
    
    
    glGenBuffersARB(1, &vbo);
    
    glBindBufferARB(GL_ARRAY_BUFFER, vbo);
    glBufferDataARB(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    
    
    // Create Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Specify layout of point data
    GLint posAttrib= shaderProgram->addAttribute("pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), 0);
    
    GLint colAttrib = shaderProgram->addAttribute("color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void*) (2 * sizeof(float)));
    
    GLint sidesAttrib = shaderProgram->addAttribute("sides");
    glEnableVertexAttribArray(sidesAttrib);
    glVertexAttribPointer(sidesAttrib, 1, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void*) (5 * sizeof(float)));
    
    return res;
}


void TestApp::Render(){
    App::Render();
    shaderProgram->use();
    
    glBindVertexArray(vao);
    
    glDrawArrays(GL_POINTS, 0, 4);
}