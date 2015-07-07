//
//  TessMeshApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 07.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "TessMeshApp.h"

#include "MeshUtils.h"
#include "ColorUtils.h"


TessMeshApp::TessMeshApp(const std::string& window_title, int window_width, int window_height):
App::App(window_title, window_width, window_height)
{
    
}



TessMeshApp::~TessMeshApp()
{
    delete mesh;
}


bool TessMeshApp::Init(){
    
    bool res = App::Init();
    if (!res) { return res; }
    
    vertexShader = new Shader(GL_VERTEX_SHADER);
    vertexShader->loadFromFile("shaders/tess.vert");
    vertexShader->compile();
    
    fragmentShader = new Shader(GL_FRAGMENT_SHADER);
    fragmentShader->loadFromFile("shaders/tess.frag");
    fragmentShader->compile();
    
    geometryShader = new Shader(GL_GEOMETRY_SHADER);
    geometryShader->loadFromFile("shaders/tess.geom");
    geometryShader->compile();
    
    tesselationControlShader = new Shader(GL_TESS_CONTROL_SHADER);
    tesselationControlShader->loadFromFile("shaders/tess.tcs");
    tesselationControlShader->compile();
    
    
    tesselationEvaluationShader = new Shader(GL_TESS_EVALUATION_SHADER);
    tesselationEvaluationShader->loadFromFile("shaders/tess.tes");
    tesselationEvaluationShader->compile();
    
    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader(*vertexShader);
    shaderProgram->attachShader(*fragmentShader);
    shaderProgram->attachShader(*geometryShader);
    shaderProgram->attachShader(*tesselationControlShader);
    shaderProgram->attachShader(*tesselationEvaluationShader);
    shaderProgram->linkProgram();
    shaderProgram->use();
    
    
    vector<vec3> vertices;
    vector<vec3> colors;
    vector<GLuint> indices;
    //    vector<Attribute>attributes;
    
    
    
    //createTetraedron(vertices, indices);
    createIcosphere(vertices, indices);
    
    
    unsigned int num_of_colors = (unsigned int)vertices.size();
    
    colors.reserve(num_of_colors);
    generateColors(num_of_colors, colors);
    
    
    mesh = new Mesh();
    // position:
    {
        Attribute positionAttrib;
        positionAttrib.name = "position";
        positionAttrib.num_of_components = 3;
        positionAttrib.data_type = GL_FLOAT;
        positionAttrib.buffer_type = GL_ARRAY_BUFFER;
        
        
        
        mesh->addVBO(vertices, positionAttrib);
        shaderProgram->use();
        positionAttrib.id = shaderProgram->addAttribute(positionAttrib.name);
        glEnableVertexAttribArray(positionAttrib.id);
        glVertexAttribPointer(positionAttrib.id, positionAttrib.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
        shaderProgram->disable();
        mesh->attributes.push_back(positionAttrib);
    }
    
    // color:
    {
        Attribute colorAttrib;
        colorAttrib.name = "color";
        colorAttrib.num_of_components = 3;
        colorAttrib.data_type = GL_FLOAT;
        colorAttrib.buffer_type = GL_ARRAY_BUFFER;
        
        
        
        mesh->addVBO(colors, colorAttrib);
        shaderProgram->use();
        colorAttrib.id = shaderProgram->addAttribute(colorAttrib.name);
        glEnableVertexAttribArray(colorAttrib.id);
        glVertexAttribPointer(colorAttrib.id, colorAttrib.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
        shaderProgram->disable();
        mesh->attributes.push_back(colorAttrib);
    }
    
    
    
    // indices:
    {
        mesh->addIndices(indices);
    }
    
    
    
    
    
    // uniforms:
    {
        shaderProgram->use();
        GLuint model = shaderProgram->addUniform("model");
        glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix));
        GLuint view = shaderProgram->addUniform("view");
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera.view));
        GLuint projection = shaderProgram->addUniform("projection");
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
        shaderProgram->disable();
    }
    
    
    
    
    
    
    return res;
}

void TessMeshApp::Update(){
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

void TessMeshApp::Render(){
    App::Render();
    shaderProgram->use();
    mesh->Draw();
    shaderProgram->disable();
}