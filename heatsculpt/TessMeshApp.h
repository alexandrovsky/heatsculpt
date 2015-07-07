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

class TessMeshApp : public App {
public:
    TessMeshApp(const std::string& window_title, int window_width, int window_height);
    virtual ~TessMeshApp();
    
    
    virtual bool Init();
    virtual void Update();
    virtual void Render();
 
private:
    
    Shader* vertexShader;
    Shader* geometryShader;
    Shader* fragmentShader;
    Shader* tesselationControlShader;
    Shader* tesselationEvaluationShader;
    
    ShaderProgram* shaderProgram;
    Mesh* mesh;
};

#endif /* defined(__heatsculpt__TessMeshApp__) */
