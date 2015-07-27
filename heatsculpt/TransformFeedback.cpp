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
    
    glGenVertexArrays(1, &vao);
}

TransformFeedback::~TransformFeedback(){
    delete shaderProgram;
    
    glDeleteVertexArrays(1, &vao);
    
    for (int i = 0; i < varyingAttributes.size(); i++) {
        
        glDeleteBuffers(1, &varyingAttributes[i]->vbo);
        glDeleteBuffers(1, &varyingAttributes[i]->destination_vbo);
        
        delete varyingAttributes[i];
        
    }
    
//    glDeleteQueries(1, &query);
    
}


bool TransformFeedback::InitShader(vector<pair<string, GLenum>> shaderSources, vector<string> varyings){
    

    shaderProgram = new ShaderProgram();
    
    
    for (int i = 0; i < shaderSources.size(); i++) {
        string shaderSrc = shaderSources[i].first;
        GLenum shaderType = shaderSources[i].second;
        
        
        Shader shader(shaderType);
        shader.loadFromFile(shaderSrc);
        shader.compile();
        
        shaderProgram->attachShader(shader);
    }
    
    // specify transform feedback output
    
    
    shaderProgram->addVaryings(varyings, GL_SEPARATE_ATTRIBS);
    PrintProgramInfoLog(shaderProgram->id());
    CheckGlErrors();
    
    
    shaderProgram->linkProgram();
    PrintProgramInfoLog(shaderProgram->id());
    


    
    return true;
}


void TransformFeedback::Update(){
    
    shaderProgram->use();
    
    //glEnable(GL_RASTERIZER_DISCARD);
    
    // bind the current vao
    glBindVertexArray(vao);
    
    for (int i = 0; i < varyingAttributes.size(); i++) {
        TransformFeedbackAttribute* attr = varyingAttributes[i];
        // bind transform feedback target
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, attr->vbo);
    }
    
    
    GLuint query;
    glGenQueries(1, &query);
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
    glBeginTransformFeedback(GL_POINTS);
    glPointSize(5.0f);
    glDrawArrays(GL_POINTS, 0, drawCount);
    glEndTransformFeedback();
    
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    GLuint primitives_written;
    glGetQueryObjectuiv( query, GL_QUERY_RESULT, &primitives_written );
    if(primitives_written > 0 )
        fprintf( stderr, "Attr: Primitives written to TFB: %d !\n", primitives_written );
    glDeleteQueries(1, &query);

    glDisable(GL_RASTERIZER_DISCARD);
    
    shaderProgram->disable();
    
    for (int i = 0; i < varyingAttributes.size(); i++) {
        TransformFeedbackAttribute* attr = varyingAttributes[i];
        std::swap(attr->vbo, attr->destination_vbo);
    }

    //glBindVertexArray(0);
    
}

void TransformFeedback::Draw(){
    
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, drawCount);
    glBindVertexArray(0);
    
}

