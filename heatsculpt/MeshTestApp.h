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



class MeshTestApp : public App{
public:
    MeshTestApp(const std::string& window_title);
    virtual ~MeshTestApp();
    
    
    virtual bool Init();
    virtual void Update();
    virtual void Render();
    
    
    Shader* vertexShader;
    Shader* geometryShader;
    Shader* fragmentShader;
    
    ShaderProgram* shaderProgram;
    Mesh* mesh;
    
};

#endif /* defined(__heatsculpt__MeshTestApp__) */
