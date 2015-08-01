//
//  Clay.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 31.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__Clay__
#define __heatsculpt__Clay__

#define kClayElementNumOfComponents 2
#define num_of_paritcles 100
#define kMaxBufferSize 65536


#include "ShaderProgram.h"
#include <glm/glm.hpp>

using namespace std;

struct ClayElement{
    glm::vec3 position;
    glm::vec3 color;
};

class Clay{
    
public:
    
    int click = 0;
    
    Clay();
    virtual ~Clay();
    bool Init();
    void Update();
    void Render(glm::mat4 view, glm::mat4 projection);
private:
    
    bool initProgram();
    bool initVertexArray();
    
    
    ShaderProgram* drawShader;
    ShaderProgram* transformfeedbackShader;
    
    
    
    // front and back buffers for the transform feedback
    GLuint vao[2];
    GLuint vbo[2];
    int current_buffer = 0;
    
    GLuint query;
    
    GLuint drawCount = num_of_paritcles;
    
};

#endif /* defined(__heatsculpt__Clay__) */
