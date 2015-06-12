//
//  App.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 12.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__CApp__
#define __heatsculpt__CApp__

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Event.h"
#include "Camera.h"

class App :public Event {
private:
    bool    Running;
    
    int width;
    int height;
    SDL_Window* displayWindow;
    SDL_Renderer* displayRenderer;
    SDL_RendererInfo displayRendererInfo;
    
    Camera camera;
    
public:
    App();
    
    int OnExecute();

    
    bool OnInit();
    
    void OnEvent(SDL_Event* Event);
    
    void OnKeyDown(SDL_Keycode key, SDL_Keymod mod);
    
    void OnKeyUp(SDL_Keycode key, SDL_Keymod mod);
    
    void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
    
    void OnExit();
    
    void OnLoop();
    
    void OnRender();
    
    void OnCleanup();
    
    
private:
    bool InitOpenGL();

};

#endif /* defined(__heatsculpt__CApp__) */
