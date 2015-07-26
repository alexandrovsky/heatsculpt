//
//  ShaderProgram.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 13.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//



#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <iostream>
#include <map>
#include <vector>
#include "Shader.h"

using std::map;
using std::string;
using std::cout;
using std::endl;

struct Attribute{
    string name;
    GLint id;
    GLuint num_of_components;
    GLuint data_type;
    GLuint vbo; // is set by the mesh
    size_t bytes;
    GLuint buffer_type;
    GLuint draw_type;
};


class ShaderProgram
{
private:
    GLuint programId;   // The unique ID / handle for the shader program
    
    // Map of attributes and their binding locations
    std::map<string,int> attributeLocList;
    
    // Map of uniforms and their binding locations
    std::map<string,int> uniformLocList;
    std::vector<Shader> shaders;
    
public:
    // Constructor
    ShaderProgram();
    
    
    // Destructor
    ~ShaderProgram();
    
    inline GLuint id(){
        return programId;
    }
    
    // Method to attach a shader to the shader program
    void attachShader(Shader shader);
    
    
    // Method to link the shader program and display the link status
    bool linkProgram();
    
    
    // Method to enable the shader program
    void use();
    
    
    // Method to disable the shader program
    void disable();
    
    
    
    // Returns the bound location of a named attribute
    GLuint attribute(const string &attribute);
    
    
    // Method to returns the bound location of a named uniform
    GLuint uniform(const string &uniform);
    
    
    // Method to add an attrbute to the shader and return the bound location
    int addAttribute(const string &attributeName);
    
    
    // Method to add a uniform to the shader and return the bound location
    int addUniform(const string &uniformName);
    void addVaryings(std::vector<const char *> varyings, GLenum bufferMode);
    
};

#endif // SHADER_PROGRAM_H


