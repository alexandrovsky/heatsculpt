//
//  Mesh.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 13.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "Mesh.h"
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(ShaderProgram* shader, const vector<Vertex>& vertices){
    
    shaderProgram = shader;
    
    transform = mat4x4();
    drawCount = (GLsizei)vertices.size();
    
    
    //shaderProgram->use();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(NUM_BUFFERS, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITION_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


    // attributes:
    GLint posAttrib = shaderProgram->addAttribute("pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    
    // uniforms:
    GLuint model = shaderProgram->addUniform("model");
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(transform));
    
    
    
    glBindVertexArray(0);
    shaderProgram->disable();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(NUM_BUFFERS, vbos);
}


void Mesh::Update(){
    
    shaderProgram->use();
    GLuint model = shaderProgram->uniform("model");
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(transform));
    
    
    shaderProgram->disable();
    
}

void Mesh::Draw() {
    
    
    shaderProgram->use();
    glBindVertexArray(vao);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITION_VERTEX_BUFFER]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
        // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, drawCount); // Starting from vertex 0; 3 vertices total -> 1 triangle
        
    glDisableVertexAttribArray(0);
    
    
    glBindVertexArray(0);
    shaderProgram->disable();
}


template<typename T> GLuint Mesh::setVBO(vector<T> vector, GLuint vbo, string attributeName){
    
    // check current vao
    GLint current_vao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
    
    if(current_vao != vao){
        glBindVertexArray(vao);
    }
    
    // generate vbo
    glGenBuffers(1, &vbo);
    
    size_t bytes = sizeof(T) * vector.size();
    
    // add data to vbo
    glBindBufferARB(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bytes, vector.data(), GL_STATIC_DRAW);
    
    
    // add attribute to shader
    GLint attribute = shaderProgram->addAttribute(attributeName);

    glEnableVertexAttribArray(attribute);
    
    // add vbo to vao
    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    return vbo;
}
