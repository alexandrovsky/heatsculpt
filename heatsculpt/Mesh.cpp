//
//  Mesh.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 13.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "Mesh.h"


Mesh::Mesh(){
    
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    transform = mat4x4();
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &colorBufferObject);
    glDeleteBuffers(1, &indexBufferObject);
}

GLuint Mesh::setVertices(vector<vec3> vertices,  string attributeName){
    return setVBO(vertices, vertexBufferObject, attributeName);
}


GLuint Mesh::setColors(vector<vec3> colors,  string attributeName){
    return setVBO(colors, colorBufferObject, attributeName);
}

GLuint Mesh::setIndices(vector<GLuint> indices,  string attributeName){
    return setVBO(indices, indexBufferObject, attributeName);
}

void Mesh::Draw(){
    shaderProgram->use();
    glEnableVertexAttribArray(vertexArrayObject);
    
    int nBufferSize = 0;
    
//    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &nBufferSize);
    
    int originalVertexArraySize = ( nBufferSize / sizeof(vec3) );
    
    glDrawArrays(GL_POINTS, 1, 3);
    
    glDisableVertexAttribArray(vertexArrayObject);
    
    shaderProgram->disable();
}


template<typename T> GLuint Mesh::setVBO(vector<T> vector, GLuint vbo, string attributeName){
    
    // check current vao
    GLint current_vao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
    
    if(current_vao != vertexArrayObject){
        glBindVertexArray(vertexArrayObject);
    }
    
    // generate vbo
    glGenBuffers(1, &vbo);
    
    size_t bytes = sizeof(T) * vector.size();
    
    // add data to vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bytes, vector.data(), GL_STATIC_DRAW);
    
    
    // add attribute to shader
    GLint attribute = shaderProgram->addAttribute(attributeName);

    glEnableVertexAttribArray(attribute);
    
    // add vbo to vao
    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    return vbo;
}
