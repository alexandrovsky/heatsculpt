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

class HeatSculptApp : public App {


public:
    
    HeatSculptApp(const std::string& window_title, bool fullscreen);
    virtual ~HeatSculptApp();
    
    
    bool Init();
    void Update();
    void Render();

   
    
private:
    Clay* clay;
};

#endif /* defined(__heatsculpt__HeatSculptApp__) */
