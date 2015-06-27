//
//  TessMeshTestApp.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 21.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TessMeshTestApp__
#define __heatsculpt__TessMeshTestApp__

#include "App.h"
#include "Mesh.h"



class TessMeshTestApp : public App{
public:
    TessMeshTestApp(const std::string& window_title, int window_width, int window_height);
    virtual ~TessMeshTestApp();
    
    
    virtual bool Init();
    virtual void Update();
    virtual void Render();
    
    
    
    Mesh* mesh;
    
    
    
    
    
    
};


#endif /* defined(__heatsculpt__TessMeshTestApp__) */
