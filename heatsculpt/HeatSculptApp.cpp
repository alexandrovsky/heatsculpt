//
//  HeatSculptApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 31.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "HeatSculptApp.h"



HeatSculptApp::HeatSculptApp(const std::string& window_title, bool fullscreen)
:App(window_title, fullscreen){
    
    clay = new Clay();
}
HeatSculptApp::~HeatSculptApp(){
    delete clay;
}


bool HeatSculptApp::Init(){
    App::Init();
    return true;
}

void HeatSculptApp::Update(){
    App::Update();

}

void HeatSculptApp::Render(){
    App::Render();

}