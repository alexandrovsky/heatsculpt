//
//  MeshTestApp.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 17.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__MeshTestApp__
#define __heatsculpt__MeshTestApp__
#include "App.h"
#include "Mesh.h"

#include <stdio.h>

class MeshTestApp : public App{
public:
    MeshTestApp(const std::string& window_title, int window_width, int window_height);
    virtual ~MeshTestApp();
    
    
    virtual bool Init();
    virtual void Update();
    virtual void Render();
    
    
    Shader* vertexShader;
    Shader* fragmentShader;
    
    ShaderProgram* shaderProgram;
    Mesh* mesh;
    
    const string vertexShaderSrc =
    GLSL(
         
         uniform mat4 model;
         uniform mat4 view;
         uniform mat4 projection;
         
         in vec3 pos;
         in vec3 color;
         
         out vec3 vColor;
         
         void main() {
             mat4 mvp = //projection *
                        //view *
                        model;
             gl_Position = vec4(pos, 1.0);
             vColor = color;
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
    
    
    
    
};

#endif /* defined(__heatsculpt__MeshTestApp__) */
