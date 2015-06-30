//
//  MeshTestApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 17.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "MeshTestApp.h"
#include "MeshUtils.h"
#include "ColorUtils.h"


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
    vector<vec3> colors;
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
    
    
    unsigned int num_of_colors = (unsigned int)vertices.size();

    colors.reserve(num_of_colors);
    generateColors(num_of_colors, colors);
    
    
    mesh = new Mesh();
    // position:
    {
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
    shaderProgram->use();
    mesh->Draw();
    shaderProgram->disable();
}