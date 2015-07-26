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
    
    for (int i = 0; i < varyingAttributs.size(); i++) {
        
        glDeleteBuffers(1, &varyingAttributs[i]->vbo);
        glDeleteBuffers(1, &varyingAttributs[i]->destination_vbo);
        
        delete varyingAttributs[i];
        
    }
    
//    glDeleteQueries(1, &query);
    
}

bool TransformFeedback::Init(){
    
    
    
    vector<const char *> varyings;
    varyings.push_back("outposition");
    
    vector<pair<string, GLenum>> shaderSources;
    shaderSources.push_back(pair<string, GLenum>("shaders/tfSimple.vert", GL_VERTEX_SHADER));
    shaderSources.push_back(pair<string, GLenum>("shaders/tfSimple.frag", GL_FRAGMENT_SHADER));
    
    InitShader(shaderSources, varyings);
    InitTransformFeedback();
    return true;
}

bool TransformFeedback::InitShader(vector<pair<string, GLenum>> shaderSources, vector<const char*> varyings){
    

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
    
    
    shaderProgram->addVaryings(varyings, GL_INTERLEAVED_ATTRIBS);
    PrintProgramInfoLog(shaderProgram->id());
    CheckGlErrors();
    
    
    shaderProgram->linkProgram();
    PrintProgramInfoLog(shaderProgram->id());
    


    
    return true;
}


bool TransformFeedback::InitTransformFeedback(){
    

    
    
    
//    for (int i = 0; i < varyingAttributs.size(); i++) {
//        TransformFeedbackAttribute* attr = varyingAttributs[i];
//        AddBuffer(vertices, *attr);
//    }
    
    //AddBuffer(vertices, positionAttrib);
    
    
    return true;
}

void TransformFeedback::Update(){
    
    shaderProgram->use();
    // bind the current vao
    glBindVertexArray(vao);
    
    for (int i = 0; i < varyingAttributs.size(); i++) {
        TransformFeedbackAttribute* attr = varyingAttributs[i];
        // bind transform feedback target
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, attr->vbo);
        
        
        
        //glEnable(GL_RASTERIZER_DISCARD);
        
        
        // perform transform feedback
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
            fprintf( stderr, "Primitives written to TFB: %d !\n", primitives_written );
        glDeleteQueries(1, &query);
        
        //glDisable(GL_RASTERIZER_DISCARD);
        
        std::swap(attr->vbo, attr->destination_vbo);
    }
    shaderProgram->disable();
    


    
}

void TransformFeedback::Draw(){
    

}

//template<typename T> void TransformFeedback::AddBuffer(vector<T>& data, TransformFeedbackAttribute* attribute){
//    
//    shaderProgram->use();
//    
//    attribute->bytes = sizeof(T) * data.size();
//    
//    glGenBuffers(1, &attribute->vbo);
//    glGenBuffers(1, &attribute->destination_vbo);
//    
//    glBindVertexArray(vao);
//    
//    // source buffer
//    {
//        glBindBuffer(attribute->buffer_type, attribute->vbo);
//        glBufferData(GL_ARRAY_BUFFER, attribute->bytes, data.data(), attribute->draw_type);
//        glEnableVertexAttribArray(attribute->id);
//        glVertexAttribPointer(attribute->id, attribute->num_of_components, attribute->data_type, GL_FALSE, 0, 0);
//    }
//    // destination buffer
//    {
//        glBindBuffer(attribute->buffer_type, attribute->destination_vbo);
//        
//        glBufferData(GL_ARRAY_BUFFER, attribute->bytes, data.data(), attribute->draw_type);
//        glEnableVertexAttribArray(attribute->id);
//        glVertexAttribPointer(attribute->id, attribute->num_of_components, attribute->data_type, GL_FALSE, 0, 0);
//    }
//    
//    glBindVertexArray(0);
//    
//    shaderProgram->disable();
//}
