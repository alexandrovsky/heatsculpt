//
//  TransformFeedback.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 24.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TransformFeedback__
#define __heatsculpt__TransformFeedback__

#include <stdio.h>
#include "ShaderProgram.h"

class TransformFeedback {
public:

    TransformFeedback();
    virtual ~TransformFeedback();

    ShaderProgram* shaderProgram;
    
    
    GLuint source_vbo;
    GLuint destination_vbo;
    
    GLuint source_vao;
    GLuint destination_vao;
    
    bool Init();
};

#endif /* defined(__heatsculpt__TransformFeedback__) */
