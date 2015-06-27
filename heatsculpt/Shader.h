//
//  Shader.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 13.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//


#ifndef SHADER_HPP
#define SHADER_HPP


#ifndef GL_TESS_CONTROL_SHADER
    #define GL_TESS_CONTROL_SHADER 0x00008e88
#endif
#ifndef GL_TESS_EVALUATION_SHADER
    #define GL_TESS_EVALUATION_SHADER 0x00008e87
#endif
#ifndef GL_PATCHES
    #define GL_PATCHES 0x0000000e
#endif
#ifndef GL_PATCH_VERTICES
    #define GL_PATCH_VERTICES 0x8E72
#endif







#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <GLFW/glfw3.h>
#define GLSL(src) "#version 400 core\n" #src

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
