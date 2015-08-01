//
//  TransformFeedback.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 24.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TransformFeedback__
#define __heatsculpt__TransformFeedback__
#include <glm/glm.hpp>
#include <utility>
#include <vector>
#include <stdio.h>
#include "ShaderProgram.h"

using namespace std;
using namespace glm;

#define buffer_count 2

struct TransformFeedbackAttribute : Attribute {
    GLuint destination_vbo;
};

class TransformFeedback {
public:

    TransformFeedback();
    virtual ~TransformFeedback();

    glm::mat4x4 modelMatrix;
    ShaderProgram* shaderProgram;
    GLsizei drawCount;

    
    GLuint vao;
    GLuint destination_vao;
    
    
    vector<TransformFeedbackAttribute*> varyingAttributes;
    
    
    inline void SetDrawCount(GLsizei count){
        drawCount = count;
    }
    
    bool InitShader(vector<pair<string, GLenum>> shaderSources, vector<string> varyings);
    void Update();
    void Draw();
    template<typename T> inline void AddBuffer(vector<T>& data, TransformFeedbackAttribute* attribute){
        shaderProgram->use();
        
        attribute->bytes = sizeof(T) * data.size();
        
        glGenBuffers(1, &attribute->vbo);
        glGenBuffers(1, &attribute->destination_vbo);
        
        glBindVertexArray(vao);
        
        // source buffer
        {
            glBindBuffer(attribute->buffer_type, attribute->vbo);
            glBufferData(GL_ARRAY_BUFFER, attribute->bytes, data.data(), attribute->draw_type);
            glEnableVertexAttribArray(attribute->id);
            glVertexAttribPointer(attribute->id, attribute->num_of_components, attribute->data_type, GL_FALSE, 0, 0);
        }
        glBindVertexArray(destination_vao);
        // destination buffer
        {
            glBindBuffer(attribute->buffer_type, attribute->destination_vbo);
            
            glBufferData(GL_ARRAY_BUFFER, attribute->bytes, data.data(), attribute->draw_type);
            glEnableVertexAttribArray(attribute->id);
            glVertexAttribPointer(attribute->id, attribute->num_of_components, attribute->data_type, GL_FALSE, 0, 0);
        }
        
        glBindVertexArray(0);
        
        shaderProgram->disable();
    }
    
private:
    
    
};

#endif /* defined(__heatsculpt__TransformFeedback__) */
