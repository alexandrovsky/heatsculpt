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

#define buffer_count 2

class TransformFeedback {
public:

    TransformFeedback();
    virtual ~TransformFeedback();

    glm::mat4x4 modelMatrix;
    ShaderProgram* shaderProgram;
    GLsizei drawCount;

    
    
    int current_buffer=0;
    GLuint vao[buffer_count];
    GLuint vbo[buffer_count];
    
//    GLuint query;
//    GLuint tfo;
//    GLuint source_vbo;
//    GLuint destination_vbo;
    
    Attribute positionAttrib;
    
    vector<glm::vec3> vertices;
    
    
    bool Init();
    void Update();
    void Draw();
    template<typename T> void addBuffer(GLuint& vbo, vector<T> data, Attribute& attribute);
    
private:
    bool InitShader();
    bool InitTransformFeedback();
    
};

#endif /* defined(__heatsculpt__TransformFeedback__) */
