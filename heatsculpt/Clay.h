//
//  Clay.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 31.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__Clay__
#define __heatsculpt__Clay__


#include "ShaderProgram.h"
#include <glm/glm.hpp>

using namespace std;

struct ClayElement{
    glm::vec3 position;
    glm::vec3 color;
};

class Clay{
    
public:
private:
    
    bool initProgram();
    bool initVertexArray();
    
    
    ShaderProgram* drawShader;
    ShaderProgram* transformfeedbackShader;
    
    
    
    // front and back buffers for the transform feedback
    GLuint vao[2];
    GLuint vbo[2];
    int current_buffer = 0;
    
    
    
};

#endif /* defined(__heatsculpt__Clay__) */
