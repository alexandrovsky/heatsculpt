//
//  Debug.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 18.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "Debug.h"
#include <OpenGL/glu.h>
#include <iostream>


void CheckGlErrors( void )
{
    GLenum e = glGetError();
    
    while ( e != GL_NO_ERROR )
    {
        fprintf( stderr, "GL error: %d %s!\n", e, gluErrorString(e) );
        e = glGetError();
    }
}




// Print out shader info (debugging!)
void PrintShaderInfoLog( const char * kind, GLuint obj )
{
    int infologLength = 0;
    glGetShaderiv( obj, GL_INFO_LOG_LENGTH, &infologLength );
    
    if ( infologLength > 0 )
    {
        char * infoLog = (char *) malloc(infologLength);
        int charsWritten  = 0;
        glGetShaderInfoLog( obj, infologLength, &charsWritten, infoLog );
        printf("%s shader log: %s\n", kind, infoLog );
        free(infoLog);
    }
    else
    {
        printf("%s shader log: OK\n", kind );
    }
}


// Print out shader program info (debugging!)
void PrintProgramInfoLog( GLuint obj )
{
    int infologLength = 0;
    glGetProgramiv( obj, GL_INFO_LOG_LENGTH, &infologLength );
    
    if ( infologLength > 0 )
    {
        char * infoLog = (char *) malloc(infologLength);
        int charsWritten  = 0;
        glGetProgramInfoLog( obj, infologLength, &charsWritten, infoLog );
        printf("Program Log: %s\n",infoLog);
        free(infoLog);
    }
    else
    {
        printf("Program Log: OK\n");
    }
}
