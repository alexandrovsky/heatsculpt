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
    glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITION_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    GLint posAttrib = shaderProgram->addAttribute("pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // --color:
    vector<vec3> colors;
    
    GLuint h = 0, s = 255, v = 255;
    unsigned char r_255 = 0, g_255 = 0, b_255 = 0;
    unsigned char maxBrightnes = 255;
    
    
    GLfloat r, g, b;
    for (int i = 0; i < vertices.size(); i++) {
        
//        r = (GLfloat)r_255/255.0f;
//        g = (GLfloat)g_255/255.0f;
//        b = (GLfloat)b_255/255.0f;
        
        if (i % 3 == 0) {
            hsv2rgb(h, s, v, &r_255, &g_255, &b_255, maxBrightnes);
            r = (GLfloat)r_255/255.0f;
            g = (GLfloat)g_255/255.0f;
            b = (GLfloat)b_255/255.0f;
            
            h += 360 / (vertices.size()/3);
            //h = h % 360;
        }
        
//        r = 1.0f;//((GLfloat)rand() / RAND_MAX) + 1;
//        g = 1.0f;//((GLfloat)rand() / RAND_MAX) + 1;
//        b = 1.0f;//((GLfloat)rand() / RAND_MAX) + 1;
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
    
    GLint posAttrib = shaderProgram->attribute("pos");
    glEnableVertexAttribArray(posAttrib);
    
    
    GLint colorAttrib = shaderProgram->attribute("color");
    glEnableVertexAttribArray(colorAttrib);
    
//    glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITION_VERTEX_BUFFER]);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[ELEMENT_ARRAY_BUFFER]);


    
    glPointSize(5.0);
    glDrawElements(GL_POINTS, drawCount, GL_UNSIGNED_INT, 0);
//    glDrawElements(GL_LINES, drawCount, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    
    
    glBindVertexArray(0);
    shaderProgram->disable();
}



