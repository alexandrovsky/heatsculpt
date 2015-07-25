//
//  TransformFeedback.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 24.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TransformFeedback__
#define __heatsculpt__TransformFeedback__

#include <vector>
#include <stdio.h>
#include "ShaderProgram.h"

using namespace std;

class TransformFeedback {
public:

    TransformFeedback();
    virtual ~TransformFeedback();

    glm::mat4x4 modelMatrix;
    ShaderProgram* shaderProgram;
    GLsizei drawCount;

    
    GLuint query;
    
    GLuint source_vbo;
    GLuint destination_vbo;
    
    GLuint source_vao;
    GLuint destination_vao;
    
    bool Init();
    void Update();
    template<typename T> void addBuffer(GLuint& vao, GLuint& vbo, vector<T> data, Attribute& attribute);
    
};

#endif /* defined(__heatsculpt__TransformFeedback__) */
