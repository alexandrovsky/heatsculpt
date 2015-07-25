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
    
    
    glGenQueries(1,&query);
    
    shaderProgram = new ShaderProgram();
    Shader vert(GL_VERTEX_SHADER);
    vert.loadFromFile("shaders/tfSimple.vert");
    vert.compile();
    
    
    Shader geom(GL_GEOMETRY_SHADER);
    geom.loadFromFile("shaders/tfSimple.geom");
    geom.compile();
    
    
    Shader frag(GL_FRAGMENT_SHADER);
    frag.loadFromFile("shaders/tfSimple.frag");
    frag.compile();
    
    shaderProgram->attachShader(vert);
    shaderProgram->attachShader(frag);
    shaderProgram->attachShader(geom);
    
    // specify transform feedback output
    const char *varyings[] = {"outposition"};
    glTransformFeedbackVaryings(this->shaderProgram->id(), 1, varyings, GL_INTERLEAVED_ATTRIBS);
    
    shaderProgram->linkProgram();

    vector<vec3> verices;
    vector<GLuint> indices;
    createIcosahedron(verices, indices);
    drawCount = verices.size();

    Attribute positionAttrib;
    positionAttrib.name = "position";
    positionAttrib.num_of_components = 3;
    positionAttrib.data_type = GL_FLOAT;
    positionAttrib.buffer_type = GL_ARRAY_BUFFER;
    positionAttrib.draw_type = GL_DYNAMIC_DRAW;

    
    addBuffer(source_vao, source_vbo, verices, positionAttrib);
    addBuffer(destination_vao, destination_vbo, verices, positionAttrib);
    
    
    
    return true;
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


template<typename T> void TransformFeedback::addBuffer(GLuint& vao, GLuint& vbo, vector<T> data, Attribute& attribute){
    
    shaderProgram->use();
    
    attribute.id = shaderProgram->addAttribute(attribute.name);
    attribute.bytes = sizeof(T) * data.size();
    
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    
    glBindBuffer(attribute.buffer_type, attribute.vbo);
    glBufferData(attribute.buffer_type, attribute.bytes, data.data(), attribute.draw_type);
    
    
    glEnableVertexAttribArray(attribute.id);
    glVertexAttribPointer(attribute.id, attribute.num_of_components, attribute.data_type, GL_FALSE, 0, 0);
    
    glBindVertexArray(0);
    shaderProgram->disable();
}
