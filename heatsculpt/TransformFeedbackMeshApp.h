//
//  TransformFeedbackMeshApp.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 24.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TransformFeedbackMeshApp__
#define __heatsculpt__TransformFeedbackMeshApp__

#include <stdio.h>
#include "App.h"
#include "TransformFeedback.h"
class TransformFeedbackMeshApp :public App{
public:
    TransformFeedbackMeshApp(const std::string& window_title, bool fullscreen);
    virtual ~TransformFeedbackMeshApp();
    
    virtual bool Init();
    virtual void Update();
    virtual void Render();
    
private:
    TransformFeedback transformFeedback;
};

#endif /* defined(__heatsculpt__TransformFeedbackMeshApp__) */
