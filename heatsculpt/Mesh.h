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

using namespace std;
using namespace glm;


class Mesh{
public:
    Mesh();
    virtual ~Mesh();
    
    GLuint vertexArrayObject;   //vao
    
    GLuint vertexBufferObject;  //vbo
    GLuint colorBufferObject;   //vbo
    GLuint indexBufferObject;   //vbo
    
    GLuint setVertices(vector<vec3> vertices, string attributeName);
    GLuint setColors(vector<vec3> colors, string attributeName);
    GLuint setIndices(vector<GLuint> indices, string attributeName);
    
//    vector<vec3> vertices;
//    vector<vec3> colors;
//    vector<GLuint> indices;
    
    ShaderProgram* shaderProgram;
    
private:
    template<typename T>GLuint setVBO(vector<T> data, GLuint vbo, string attributeName);
    
};
#endif /* defined(__heatsculpt__Mesh__) */
