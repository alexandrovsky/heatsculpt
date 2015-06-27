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

#include <gl/glew.h>
#include <OpenGL/OpenGL.h>

#include "ShaderProgram.h"


using namespace std;
using namespace glm;



struct Attribute{
    string name;
    GLuint num_of_components;
    GLuint type;
    GLuint vbo; // is set by the mesh
    GLuint buffertype;
};


struct Uniform{
    string name;
    GLuint num_of_components;
    GLuint type;
};


class Mesh{
public:
    Mesh(ShaderProgram* shader, const vector<vec3>& vertices, const vector<GLuint>& indices);
    Mesh(ShaderProgram* shader, vector<Attribute> attributes, const vector<vec3>& vertices, const vector<GLuint>& indices);
    virtual ~Mesh();
    
    mat4x4 modelMatrix;
    mat4x4 viewMatrix;
    mat4x4 projectionMatrix;
    
    void Update();
    void Draw();
    
    template<typename T> GLuint addVBO(vector<T> vector, GLuint vbo, string attributeName, GLuint type=GL_ARRAY_BUFFER);

    
    ShaderProgram* shaderProgram;
    vector<Attribute> attributes;

    inline GLsizei getDrawCount(){
        return drawCount;
    }
    
private:
    
    
    
    
    enum {
        POSITION_VERTEX_BUFFER,
        COLOR_VERTEX_BUFFER,
        ELEMENT_ARRAY_BUFFER,
        NUM_BUFFERS // must be last element in enum
    };
    
    GLuint vao;
    GLuint vbos[NUM_BUFFERS];

    Attribute indicesAttrib;
    GLsizei drawCount;
    
    bool use_vbo_array; 
    

    
};
#endif /* defined(__heatsculpt__Mesh__) */
