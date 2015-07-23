//
//  TransformFeedbackTessApp.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 20.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TransformFeedbackTessApp__
#define __heatsculpt__TransformFeedbackTessApp__

#include <stdio.h>

#include "App.h"
#include "ShaderProgram.h"
#include "TransformFeedbackMesh.h"

class TransformFeedbackTessApp : public App {
public:
    
    TransformFeedbackTessApp(const std::string& window_title, bool fullscreen);
    virtual ~TransformFeedbackTessApp();
    
    virtual bool Init();
    virtual void Update();
    virtual void Render();
    
private:
    TransformFeedbackMesh* mesh;
    
//    GLuint tfSourceVBO, tfDestinationVBO;
//    GLuint tfSourceVAO, tfDestinationVAO;
    
    ShaderProgram* transformFeedbackShader;
    ShaderProgram* tesselationShader;
    
    bool InitMesh();
    bool InitTransformFeedbackShader();
    bool InitTesselationShader();
    
};

#endif /* defined(__heatsculpt__TransformFeedbackTessApp__) */
