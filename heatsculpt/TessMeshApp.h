//
//  TessMeshApp.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 07.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TessMeshApp__
#define __heatsculpt__TessMeshApp__
#include "App.h"
#include "Mesh.h"
#include "ShaderProgram.h"
class TessMeshApp : public App {
public:
    TessMeshApp(const std::string& window_title, bool fullscreen);
    virtual ~TessMeshApp();
    
    
    virtual bool Init();
    virtual void Update();
    virtual void Render();
 
private:
    
    void initMeshShader();
    void initMesh();
    
    ShaderProgram* meshShaderProgram;
    ShaderProgram* mouseShaderProgram;
    
    Mesh* mesh;
    Mesh* mouse;
    
};

#endif /* defined(__heatsculpt__TessMeshApp__) */
