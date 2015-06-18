//
//  Mesh.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 13.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__Mesh__
#define __heatsculpt__Mesh__

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include <GLEW/GLEW.h>
#include <OpenGL/OpenGL.h>

#include "ShaderProgram.h"

#include "Vertex.h"

using namespace std;
using namespace glm;


class Mesh{
public:
    Mesh(ShaderProgram* shader, const vector<Vertex>& vertices);
    virtual ~Mesh();
    
    mat4x4 modelMatrix;
    mat4x4 viewMatrix;
    mat4x4 projectionMatrix;
    
    void Update();
    void Draw();
    

    
    ShaderProgram* shaderProgram;
    
private:
    
    enum {
       POSITION_VERTEX_BUFFER,
        NUM_BUFFERS // must be last element in enum
    };
    
    GLuint vao;
    GLuint vbos[NUM_BUFFERS];
    
    template<typename T>GLuint setVBO(vector<T> data, GLuint vbo, string attributeName);
    
    GLsizei drawCount;

    
};
#endif /* defined(__heatsculpt__Mesh__) */
