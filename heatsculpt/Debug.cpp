//
//  Debug.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 18.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "Debug.h"

#include <OpenGL/glu.h>
void checkGlErrors( void )
{
    GLenum e = glGetError();
    while ( e != GL_NO_ERROR )
    {
        fprintf( stderr, "GL error: %s!\n", gluErrorString(e) );
        e = glGetError();
    }
}