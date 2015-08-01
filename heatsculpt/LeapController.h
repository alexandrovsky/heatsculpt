//
//  LeapController.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 01.08.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__LeapController__
#define __heatsculpt__LeapController__

#include <stdio.h>
#include "Leap.h"

#include "Mesh.h"
#include "ShaderProgram.h"

#define num_of_finger_points 10 // 5 fingers * 4 * bonese * 2 hands

using namespace Leap;

class LeapController : public Listener {
public:
    LeapController();
    ~LeapController();
    void Init();
    void Destroy();
    void Render(mat4 view, mat4 projection);
    
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);
    
private:
    Frame frame;
    
    mat4 modelMatrix;
    Controller controller;
    Mesh* hands;
    ShaderProgram* handsDrawShader;
    
    vector<vec3> fingerVertices;
    Attribute positionAttrib;
    Attribute colorAttrib;
    
    
};
#endif /* defined(__heatsculpt__LeapController__) */
