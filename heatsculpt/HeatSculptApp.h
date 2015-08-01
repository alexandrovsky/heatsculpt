//
//  HeatSculptApp.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 31.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__HeatSculptApp__
#define __heatsculpt__HeatSculptApp__

#include <stdio.h>
#include "App.h"
#include "Clay.h"
#include "Leap.h"
#include "LeapController.h"


class HeatSculptApp : public App {


public:
    
    HeatSculptApp(const std::string& window_title, bool fullscreen);
    virtual ~HeatSculptApp();
    
    
    bool Init();
    void Update();
    void Render();
    void Exit();
    
//    void OnKeyUp(GLFWwindow* window, int key, int scancode, int action, int mods);
   
    
private:

    Clay* clay;
    LeapController leap;
    Controller controller;
};

#endif /* defined(__heatsculpt__HeatSculptApp__) */
