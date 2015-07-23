//
//  TestApp.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 15.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TestApp__
#define __heatsculpt__TestApp__

#include "App.h"
#include "ShaderProgram.h"
#include <stdio.h>

class TestApp : public App{
public:
    TestApp(const std::string& window_title);
    virtual ~TestApp();

    
    virtual bool Init();
    virtual void Render();
    
    
    GLuint vbo;
    GLuint vao;
    
    Shader* vertexShader;
    Shader* geometryShader;
    Shader* fragmentShader;
    
    ShaderProgram* shaderProgram;
    

    
    const string vertexShaderSrc =
    GLSL(
         in vec3 pos;
         in vec3 color;
         in float sides;
         
         out vec3 vColor;
         out float vSides;
         
         void main() {
             
             gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
             vColor = color;
             vSides = sides;
         }
    );
    
    // fragment:
    const char* fragmentShaderSrc =
    GLSL(
         out vec4 outColor;
         void main() {
             outColor = vec4(0.8, 0.2, 0.2, 1.0);
             
         }
    );
    
    
    // geometry
    const char* geometryShaderSrc =
    GLSL(
         layout(points) in;
         layout(line_strip, max_vertices = 64) out;
         
         in vec3 vColor[];
         in float vSides[];
         
         out vec3 fColor;
         
         const float PI = 3.1415926;
         
         void main() {
             fColor = vColor[0];
             
             for (int i = 0; i <= vSides[0]; i++) {
                 // Angle between each side in radians
                 float s = vSides[0];
                 float ang = PI * 2.0 / s * i;
                 
                 // Offset from center of point (0.3 to accomodate for aspect ratio)
                 vec4 offset = vec4(cos(ang) * 0.3, -sin(ang) * 0.4, 0.0, 0.0);
                 gl_Position = gl_in[0].gl_Position + offset;
                 
                 EmitVertex();
             }
             
             EndPrimitive();
         }
    );
    
};


#endif /* defined(__heatsculpt__TestApp__) */
