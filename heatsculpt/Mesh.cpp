//
//  Mesh.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 13.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "Mesh.h"
#include <glm/gtc/type_ptr.hpp>
#include "ColorUtils.h"

Mesh::Mesh(ShaderProgram* shader, const vector<vec3>& vertices, const vector<GLuint>& indices){
    
    shaderProgram = shader;
    
    modelMatrix = mat4x4();
    viewMatrix = mat4x4();
    projectionMatrix = mat4x4();
    
    drawCount = (GLsizei)vertices.size();
    
    
    //shaderProgram->use();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(NUM_BUFFERS, vbos);


    // attributes:
    
    // --position:
    
    addVBO(vertices, vbos[POSITION_VERTEX_BUFFER], "Position");
    
    // --color:
    unsigned int num_of_colors = (unsigned int)vertices.size();
    vector<vec3> colors;
    colors.reserve(num_of_colors);
    generateColors(num_of_colors, colors);
    
    addVBO(colors, vbos[COLOR_VERTEX_BUFFER], "Color");

    
    // -----
    
    
    
    // indices:
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[ELEMENT_ARRAY_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
#warning resize drawCount
    drawCount = (GLsizei)indices.size(); //
    
    // -----
    
    
    // uniforms:
    GLuint model = shaderProgram->addUniform("model");
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    
    GLuint view = shaderProgram->addUniform("view");
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    
    GLuint projection = shaderProgram->addUniform("projection");
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    
    
    glBindVertexArray(0);
    shaderProgram->disable();
    
    
    use_vbo_array = true;
}



Mesh::Mesh(ShaderProgram* shader, vector<Attribute> attributes, const vector<vec3>& vertices, const vector<GLuint>& indices){
    
    
    
    this->shaderProgram = shader;
    this->attributes = attributes;
    
    this->modelMatrix = mat4x4();
    this->viewMatrix = mat4x4();
    this->projectionMatrix = mat4x4();
    
    drawCount = (GLsizei)vertices.size();
    
    
    //shaderProgram->use();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbosHandle[this->attributes.size()];
    
    glGenBuffers((GLsizei)this->attributes.size(), vbosHandle);
    for (int i = 0; i < attributes.size(); i++) {
#warning set the right array instead of vertices
        this->attributes[i].vbo = vbosHandle[i];
//        addVBO(vertices, this->attributes[i].vbo, this->attributes[i].name);
        
        
        addVBO(vertices, this->attributes[i]);
        ;
    }
    


    indicesAttrib.name = "Index";
    indicesAttrib.num_of_components = 1;
    indicesAttrib.data_type = GL_UNSIGNED_INT;
    indicesAttrib.buffer_type = GL_ELEMENT_ARRAY_BUFFER;
    
    
    glGenBuffers(1, &indicesAttrib.vbo);
    // indices:
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesAttrib.vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
#warning resize drawCount
    drawCount = (GLsizei)indices.size(); //
    
    // -----
    
    glBindVertexArray(0);
    shaderProgram->disable();
    
    use_vbo_array = false;
}




Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(NUM_BUFFERS, vbos);
}


void Mesh::Update(){
    
    shaderProgram->use();{
        
//        mat4 modelview = viewMatrix * modelMatrix;
//        GLuint model = shaderProgram->uniform("Modelview");
//        glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(modelMatrix));
//        
//
//        GLuint projection = shaderProgram->uniform("Projection");
//        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        
        GLuint model = shaderProgram->uniform("model");
        glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        
        GLuint view = shaderProgram->uniform("view");
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        
        GLuint projection = shaderProgram->uniform("projection");
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    }shaderProgram->disable();
    
}

void Mesh::Draw() {
    
    
    shaderProgram->use();
    glBindVertexArray(vao);
    
    


    if (use_vbo_array) {
        GLint posAttrib = shaderProgram->attribute("Position");
        glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITION_VERTEX_BUFFER]);
        glEnableVertexAttribArray(posAttrib);
        
        
        GLint colorAttrib = shaderProgram->attribute("Color");
        glBindBuffer(GL_ARRAY_BUFFER, vbos[COLOR_VERTEX_BUFFER]);
        glEnableVertexAttribArray(colorAttrib);
        
        
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[ELEMENT_ARRAY_BUFFER]);
    }else{
        
            for (int i = 0; i < attributes.size(); i++) {
                Attribute attrib = attributes[i];
                glBindBuffer(GL_ARRAY_BUFFER, attributes[i].vbo);
                glEnableVertexAttribArray(shaderProgram->attribute(attrib.name));
            }
        
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesAttrib.vbo);
    }
    
    
    
    

    
    glPointSize(5.0);
    glDrawElements(GL_POINTS, drawCount, GL_UNSIGNED_INT, 0);
//    glDrawElements(GL_LINES, drawCount, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    
    
    glBindVertexArray(0);
    shaderProgram->disable();
}


template<typename T> GLuint Mesh::addVBO(vector<T> vector, GLuint vbo, string attributeName, GLuint type){
    
    // check current vao
    GLint current_vao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
    
    if(current_vao != vao){
        glBindVertexArray(vao);
    }
    
    
    size_t bytes = sizeof(T) * vector.size();
    glGenBuffers(1, &vbo);
    // add data to vbo
    glBindBufferARB(type, vbo);
    glBufferData(type, bytes, vector.data(), GL_STATIC_DRAW);
    
    
    // add attribute to shader
    GLint attribute = shaderProgram->addAttribute(attributeName);
    
    glEnableVertexAttribArray(attribute);
    
    // add vbo to vao
    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    return vbo;
}



template<typename T> GLuint Mesh::addVBO(vector<T> vector, Attribute& attribute){
    
    // check current vao
    GLint current_vao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
    
    if(current_vao != vao){
        glBindVertexArray(vao);
    }
    
    
    attribute.bytes = sizeof(T) * vector.size();
    glGenBuffers(1, &attribute.vbo);
    // add data to vbo
    glBindBufferARB(attribute.buffer_type, attribute.vbo);
    glBufferData(attribute.buffer_type, attribute.bytes, vector.data(), GL_STATIC_DRAW);
    
    
    // add attribute to shader
    attribute.id = shaderProgram->addAttribute(attribute.name);
    
    glEnableVertexAttribArray(attribute.id);
    
    // add vbo to vao
    glVertexAttribPointer(attribute.id, attribute.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
    
    return attribute.vbo;
}

