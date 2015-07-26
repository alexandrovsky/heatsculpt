//
//  TransformFeedback.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 24.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TransformFeedback__
#define __heatsculpt__TransformFeedback__
#include <utility>
#include <vector>
#include <stdio.h>
#include "ShaderProgram.h"

using namespace std;

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
    
    
    //TransformFeedbackAttribute positionAttrib;
    
    vector<glm::vec3> vertices;
    vector<TransformFeedbackAttribute*> varyingAttributs;
    
    
    
    
    bool InitShader(vector<pair<string, GLenum>> shaderSources, vector<const char*> varyings);
    bool InitTransformFeedback();
    
    bool Init();
    void Update();
    void Draw();
    template<typename T> void AddBuffer(vector<T> data, TransformFeedbackAttribute& attribute);
    
private:
    
    
};

#endif /* defined(__heatsculpt__TransformFeedback__) */
