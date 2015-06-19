//
//  Mesh.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 13.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "Mesh.h"
#include <glm/gtc/type_ptr.hpp>

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
    glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITION_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    GLint posAttrib = shaderProgram->addAttribute("pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // --color:
    vector<vec3> colors;
    for (int i = 0; i < vertices.size(); i++) {
        float r = ((float)rand() / RAND_MAX) + 1;
        float g = ((float)rand() / RAND_MAX) + 1;
        float b = ((float)rand() / RAND_MAX) + 1;
        colors.push_back(glm::vec3(r, g, b));
    }
    
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[COLOR_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors[0]) * colors.size(), colors.data(), GL_STATIC_DRAW);
    
    GLint colorAttrib = shaderProgram->addAttribute("color");
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
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
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(NUM_BUFFERS, vbos);
}


void Mesh::Update(){
    
    shaderProgram->use();{
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
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITION_VERTEX_BUFFER]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[ELEMENT_ARRAY_BUFFER]);
//    glDrawElements(GL_TRIANGLE_STRIP, drawCount, GL_UNSIGNED_INT, 0);

    glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
