//
//  Debug.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 18.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__Debug__
#define __heatsculpt__Debug__
#include <OpenGL/OpenGL.h>
#include <vector>

#include <stdio.h>


#include <string>


std::vector<std::string> GetFirstNMessages(GLuint numMsgs);
void CheckGlErrors( void );

// Print out shader info (debugging!)
void PrintShaderInfoLog( const char * kind, GLuint obj );


// Print out shader program info (debugging!)
void PrintProgramInfoLog( GLuint obj );


#endif /* defined(__heatsculpt__Debug__) */
