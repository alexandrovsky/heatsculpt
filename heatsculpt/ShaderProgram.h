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

#include "Shader.h"

using std::map;
using std::string;
using std::cout;
using std::endl;


class ShaderProgram
{
private:
    GLuint programId;   // The unique ID / handle for the shader program
    GLuint shaderCount; // How many shaders are attached to the shader program
    
    // Map of attributes and their binding locations
    std::map<string,int> attributeLocList;
    
    // Map of uniforms and their binding locations
    std::map<string,int> uniformLocList;
    
public:
    // Constructor
    ShaderProgram();
    
    
    // Destructor
    ~ShaderProgram();
    
    
    
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
    
};

#endif // SHADER_PROGRAM_H


