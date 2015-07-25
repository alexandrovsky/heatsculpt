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
    GLuint tfo;
    GLuint source_vbo;
    GLuint destination_vbo;
    
    Attribute positionAttrib;
    
    vector<glm::vec2> vertices;
    
    
    bool Init();
    void Update();
    void Draw();
    template<typename T> void addBuffer(GLuint& vbo, vector<T> data, Attribute& attribute);
    
private:
    bool InitShader();
    bool InitTransformFeedback();
    
};

#endif /* defined(__heatsculpt__TransformFeedback__) */
