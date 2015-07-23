//
//  Mesh.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 13.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__Mesh__
#define __heatsculpt__Mesh__
#include <string>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include <gl/glew.h>
#include <OpenGL/OpenGL.h>

//#include "ShaderProgram.h"


using namespace std;
using namespace glm;



struct Attribute{
    string name;
    GLuint id;
    GLuint num_of_components;
    GLuint data_type;
    GLuint vbo; // is set by the mesh
    size_t bytes;
    GLuint buffer_type;
};


class Mesh{
public:
    Mesh();
    Mesh(const vector<vec3>& vertices, const vector<GLuint>& indices);

    virtual ~Mesh();
    
    mat4x4 modelMatrix;

    
    void Update(GLfloat deltaTime);
    void Draw(GLuint type);
    
    void addIndices(vector<GLuint> indices);
    template<typename T> GLuint addVBO(vector<T> vector, Attribute& attribute, GLuint used_vao);
    
    void setupAttribute(GLuint vao, Attribute attribute);

    vector<Attribute> attributes;

    
    inline GLsizei getDrawCount(){
        return drawCount;
    }
    
    inline GLuint getVAO(){
        return vao;
    }
protected:
    Attribute indicesAttrib;
    GLsizei drawCount;
    GLuint vao;
private:
    
    
    
    

    
};
#endif /* defined(__heatsculpt__Mesh__) */
