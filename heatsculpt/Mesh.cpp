//
//  Mesh.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 13.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "Mesh.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ColorUtils.h"


Mesh::Mesh(){
    
    attributes.clear();
    this->modelMatrix = mat4x4();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glBindVertexArray(0);

}

Mesh::Mesh(const vector<vec3>& vertices, const vector<GLuint>& indices){
    attributes.clear();
    this->modelMatrix = mat4x4();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    Attribute vertexAttrib;
    
    addVBO(vertices, vertexAttrib);

    addIndices(indices);

    

    glBindVertexArray(0);
    
}


Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);

}


void Mesh::Update(){
    modelMatrix = glm::rotate( mat4(), glm::radians(0.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
    
}

void Mesh::Draw(GLuint type) {

    glBindVertexArray(vao);


    for (int i = 0; i < attributes.size(); i++) {
        Attribute attrib = attributes[i];
        glBindBuffer(attributes[i].buffer_type, attributes[i].vbo);
        glEnableVertexAttribArray(attrib.id);
    }
    
    
    glBindBuffer(indicesAttrib.buffer_type, indicesAttrib.vbo);

    
//    glPointSize(5.0);
//    glDrawElements(GL_POINTS, drawCount, indicesAttrib.data_type, 0);
//    glDrawElements(GL_LINES, drawCount, GL_UNSIGNED_INT, 0);
    
    
//    glDrawElements(GL_TRIANGLES, drawCount, indicesAttrib.data_type, 0);
    
    switch (type) {
        case GL_PATCHES:
            glPatchParameteri(GL_PATCH_VERTICES, 3);
            break;
        case GL_POINTS:
            glPointSize(5.0);
            break;
        default:
            break;
    }

    glDrawElements(type, drawCount, indicesAttrib.data_type, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    
    
    glBindVertexArray(0);
}



void Mesh::addIndices(vector<GLuint> indices){
    
    
    // check current vao
    GLint current_vao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
    
    if(current_vao != vao){
        glBindVertexArray(vao);
    }
    
    indicesAttrib.name = "Index";
    indicesAttrib.num_of_components = 1;
    indicesAttrib.data_type = GL_UNSIGNED_INT;
    indicesAttrib.buffer_type = GL_ELEMENT_ARRAY_BUFFER;
    indicesAttrib.bytes = sizeof(indices[0]) * indices.size();
    
    glGenBuffers(1, &indicesAttrib.vbo);
    // indices:
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesAttrib.vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesAttrib.bytes, indices.data(), GL_STATIC_DRAW);
    
    
#warning resize drawCount
    drawCount = (GLsizei)indices.size(); //

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
    

    return attribute.vbo;
}

