//
//  TransformFeedback.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 24.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//
#include <gl/glew.h>
#include <glm/vec3.hpp>
#include "MeshUtils.h"
#include "TransformFeedback.h"


TransformFeedback::TransformFeedback(){
}

TransformFeedback::~TransformFeedback(){
    delete shaderProgram;
}


bool TransformFeedback::Init(){
    
    shaderProgram = new ShaderProgram();
    Shader vert(GL_VERTEX_SHADER);
    vert.loadFromFile("shaders/tfSimple.vert");
    vert.compile();
    
    
    Shader frag(GL_FRAGMENT_SHADER);
    frag.loadFromFile("shaders/tfSimple.frag");
    frag.compile();
    
    shaderProgram->attachShader(vert);
    shaderProgram->attachShader(frag);
    
    // specify transform feedback output
    const char *varyings[] = {"outposition"};
    glTransformFeedbackVaryings(this->shaderProgram->id(), 1, varyings, GL_INTERLEAVED_ATTRIBS);
    
    shaderProgram->linkProgram();

    vector<vec3> verices;
    vector<GLuint> indices;
    createIcosahedron(verices, indices);
    

    Attribute positionAttrib;
    positionAttrib.name = "position";
    positionAttrib.num_of_components = 3;
    positionAttrib.data_type = GL_FLOAT;
    positionAttrib.buffer_type = GL_ARRAY_BUFFER;
    
    
    addBuffer(source_vao, source_vbo, verices, positionAttrib);
    addBuffer(destination_vao, destination_vbo, verices, positionAttrib);
    
    
    
    return true;
}

void TransformFeedback::Update(){
    
}


template<typename T> void TransformFeedback::addBuffer(GLuint& vao, GLuint& vbo, vector<T> data, Attribute& attribute){
    
    shaderProgram->use();
    
    int positionAttrib = shaderProgram->addAttribute("position");
    attribute.bytes = sizeof(T) * data.size();
    
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    
    glBindBufferARB(attribute.buffer_type, attribute.vbo);
    glBufferData(attribute.buffer_type, attribute.bytes, data.data(), GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(attribute.id);
    glVertexAttribPointer(attribute.id, attribute.num_of_components, attribute.data_type, GL_FALSE, 0, 0);
    
    glBindVertexArray(0);
    shaderProgram->disable();
}
