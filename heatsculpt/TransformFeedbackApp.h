//
//  TransformFeedbackApp.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 15.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TransformFeedbackApp__
#define __heatsculpt__TransformFeedbackApp__

#include "App.h"
#include "ShaderProgram.h"


#define buffercount 2

class TransformFeedbackApp : public App {
public:
    TransformFeedbackApp(const std::string& window_title, bool fullscreen);
    virtual ~TransformFeedbackApp();
    
    
    virtual bool Init();
    virtual void Update();
    virtual void Render();

private:
    ShaderProgram* drawShader;
    ShaderProgram* transformfeedbackShader;
    
    GLint center_location;
    GLint radius_location;
    GLint g_location;
    GLint dt_location;
    GLint bounce_location;
    GLint seed_location;
    
    
    GLint View_location;
    GLint Projection_location;
    
    const int particles = 128*1024;
    int current_buffer=0;
    
    GLuint vao[buffercount];
    GLuint vbo[buffercount];

    
};

#include <stdio.h>

#endif /* defined(__heatsculpt__TransformFeedbackApp__) */
