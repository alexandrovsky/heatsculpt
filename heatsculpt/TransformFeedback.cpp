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
#include "Debug.h"

TransformFeedback::TransformFeedback():modelMatrix(1){
}

TransformFeedback::~TransformFeedback(){
    delete shaderProgram;
    
    glDeleteQueries(1, &query);
    
}

bool TransformFeedback::Init(){
    InitShader();
    InitTransformFeedback();
    return true;
}

bool TransformFeedback::InitShader(){
    
    
    
    shaderProgram = new ShaderProgram();
    Shader vert(GL_VERTEX_SHADER);
    vert.loadFromFile("shaders/tfSimple.vert");
    vert.compile();
    
    
    Shader geom(GL_GEOMETRY_SHADER);
    geom.loadFromFile("shaders/tfSimple.geom");
    geom.compile();
    
    
//    Shader frag(GL_FRAGMENT_SHADER);
//    frag.loadFromFile("shaders/tfSimple.frag");
//    frag.compile();
    
    shaderProgram->attachShader(vert);
//    shaderProgram->attachShader(frag);
    shaderProgram->attachShader(geom);
    
    // specify transform feedback output
    const char *varyings[] = {"outPosition"};
    glTransformFeedbackVaryings(this->shaderProgram->id(), 1, varyings, GL_INTERLEAVED_ATTRIBS);
    
    shaderProgram->linkProgram();
    PrintProgramInfoLog(shaderProgram->id());
    


    
    positionAttrib.name = "position";
    positionAttrib.num_of_components = 2;
    positionAttrib.data_type = GL_FLOAT;
    positionAttrib.buffer_type = GL_ARRAY_BUFFER;
    positionAttrib.draw_type = GL_DYNAMIC_DRAW;

    positionAttrib.id = shaderProgram->addAttribute(positionAttrib.name);
    
    return true;
}


bool TransformFeedback::InitTransformFeedback(){
    
    glGenQueries(1,&query);
    
    vector<vec2> verices;
    float v = 0.0f;
    for (int i = 0; i < 10; i++) {
        verices.push_back(vec2(v, v));
        v += 0.1;
    }
    
    drawCount = (GLsizei)verices.size();
    
    glGenBuffers(1, &source_vbo);
    
    glGenBuffers(1, &destination_vbo);
    glBindBuffer(positionAttrib.buffer_type, destination_vbo);
    
    
    
}

void TransformFeedback::Update(){
    
    
    shaderProgram->use();
    PrintProgramInfoLog(shaderProgram->id());
    // set uniforms:
    {
    }
    glEnable(GL_RASTERIZER_DISCARD);
    
    // bind the current vao
    glBindVertexArray(source_vao);
    glBindBuffer(GL_ARRAY_BUFFER, source_vbo);
    
    PrintProgramInfoLog(shaderProgram->id());
    
    // bind transform feedback target
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, destination_vbo);
    
    PrintProgramInfoLog(shaderProgram->id());
    
    CheckGlErrors();
    
    // perform transform feedback
    glBeginQuery( GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query );
    glBeginTransformFeedbackEXT(GL_POINTS);
    
    
    PrintProgramInfoLog(shaderProgram->id());
    CheckGlErrors();
    
    
    
    glDrawArrays(GL_POINTS, 0, this->drawCount);
    CheckGlErrors();
    PrintProgramInfoLog(shaderProgram->id());
    glEndTransformFeedback();
    
    CheckGlErrors();
    PrintProgramInfoLog(shaderProgram->id());
    
    glEndQuery( GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN );
    
    CheckGlErrors();
    
    GLuint primitives_written;
    // read back query results
    glGetQueryObjectuiv( query, GL_QUERY_RESULT, &primitives_written );
    fprintf( stderr, "Primitives written to TFB: %d !\n", primitives_written );
    
    
    glDisable(GL_RASTERIZER_DISCARD);
    
    shaderProgram->disable();
    
    std::swap(source_vao, destination_vao);
    std::swap(source_vbo, destination_vbo);
    
    //    GetFirstNMessages(1);
    CheckGlErrors();

    
}


template<typename T> void TransformFeedback::addBuffer(GLuint& vbo, vector<T> data, Attribute& attribute){
    
    shaderProgram->use();
    
    attribute.id = shaderProgram->addAttribute(attribute.name);
    attribute.bytes = sizeof(T) * data.size();
    
    
    glBindBuffer(attribute.buffer_type, vbo);
    glBufferData(attribute.buffer_type, attribute.bytes, data.data(), attribute.draw_type);
    
    
    glEnableVertexAttribArray(attribute.id);
    glVertexAttribPointer(attribute.id, attribute.num_of_components, attribute.data_type, GL_FALSE, 0, 0);
    
    glBindVertexArray(0);
    shaderProgram->disable();
}
