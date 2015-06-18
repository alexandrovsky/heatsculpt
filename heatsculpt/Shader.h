//
//  Shader.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 13.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//


#ifndef SHADER_HPP
#define SHADER_HPP


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <GLFW/glfw3.h>
#define GLSL(src) "#version 330 core\n" #src

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::ifstream;

class Shader
{
private:
    GLuint id;         // The unique ID / handle for the shader
    std::string typeString; // String representation of the shader type (i.e. "Vertex" or such)
    std::string source;     // The shader source code (i.e. the GLSL code itself)
    
public:
    // Constructor
    Shader(const GLenum &type);
    virtual ~Shader();
    
    inline GLuint getId()
    {
        return id;
    }
    
    inline string getSource()
    {
        return source;
    }
    
    // Method to load the shader contents from a file
    bool loadFromString(const string &sourceString);
    
    // Method to load the shader contents from a string
    bool loadFromFile(const string &filename);
    
    
    // Method to compile a shader and display any problems if compilation fails
    bool compile();
};


#endif /* defined(__heatsculpt__Shader__) */
