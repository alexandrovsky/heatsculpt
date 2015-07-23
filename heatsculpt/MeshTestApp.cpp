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


MeshTestApp::MeshTestApp(const std::string& window_title):
App::App(window_title, true)
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
    vertexShader->loadFromFile("shaders/example1.vert");
    vertexShader->compile();
    
    fragmentShader = new Shader(GL_FRAGMENT_SHADER);
    fragmentShader->loadFromFile("shaders/example1.frag");
    fragmentShader->compile();
    
    geometryShader = new Shader(GL_GEOMETRY_SHADER);
    geometryShader->loadFromFile("shaders/example1.geom");
    geometryShader->compile();
    
    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader(*vertexShader);
    shaderProgram->attachShader(*fragmentShader);
    shaderProgram->attachShader(*geometryShader);
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

        
        
        mesh->addVBO(vertices, positionAttrib, mesh->getVAO());
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

        
        
        mesh->addVBO(colors, colorAttrib, mesh->getVAO());
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

    
    mesh->Update(glfwGetTime());
    
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
    mesh->Draw(GL_TRIANGLES);
    shaderProgram->disable();
}