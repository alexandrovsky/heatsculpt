//
//  App.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 12.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "App.h"
#include <OpenGL/glu.h>

App::App():width(800),height(600) {
    
    displayWindow = NULL;
    displayRenderer = NULL;
    
    Running = true;
}

int App::OnExecute() {
    if(OnInit() == false) {
        return -1;
    }
    
    SDL_Event Event;
    
    while(Running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }
        
        OnLoop();
        OnRender();
    }
    
    OnCleanup();
    
    return 0;
}

bool App::InitOpenGL(){
    
    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH );
    
    /* Set the background black */
    glClearColor( 1.0f, 0.0f, 0.0f, 0.0f );
    
    /* Depth buffer setup */
    glClearDepth( 1.0f );
    
    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );

    /* The Type Of Depth Test To Do */
    glEnable(GL_CULL_FACE);
    glDepthFunc( GL_LEQUAL );
    
    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    
    return true;
}



bool App::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
    SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
    /*TODO: Check that we have OpenGL */
    if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        return false;
    }

    
    
    if (!InitOpenGL()) {
        return false;
    }
    

    
    camera.SetMode(FREE);	//Two Modes FREE and ORTHO
    camera.SetPosition(glm::vec3(0, 0, -10));
    camera.SetViewport(0, 0, width, height);
    camera.SetLookAt(glm::vec3(0, 0, 0));
    camera.SetClipping(.01, 1000);
    camera.SetFOV(45);
    
    return true;
}


void App::OnKeyDown(SDL_Keycode key, SDL_Keymod mod){
    switch (key) {
        case SDLK_LEFT:
            camera.Move(LEFT);
            break;
        case SDLK_RIGHT:
            camera.Move(RIGHT);
            break;
        case SDLK_UP:
            camera.Move(FORWARD);
            break;
        case SDLK_DOWN:
            camera.Move(BACK);
            break;
        default:
            break;
    }
}

void App::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle){
    if (!Left && !Middle && !Right) {
        camera.SetPos(3, Left, mX, mY);
    } else {
        camera.Move2D(mX, mY);
    }
}

void App::OnKeyUp(SDL_Keycode sym, SDL_Keymod mod){
}

void App::OnExit(){
    Running = false;
}

void App::OnEvent(SDL_Event* Event) {
    Event::OnEvent(Event);
}

void App::OnLoop() {
    
}

void App::OnRender() {
    
    camera.Update();
    
    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );
    
    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();


    
    glBegin( GL_QUADS );                /* Draw A Quad */
    glVertex3f( -1.0f,  1.0f, 0.0f ); /* Top Left */
    glVertex3f(  1.0f,  1.0f, 0.0f ); /* Top Right */
    glVertex3f(  1.0f, -1.0f, 0.0f ); /* Bottom Right */
    glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
    glEnd( );                           /* Done Drawing The Quad */
    
    SDL_RenderPresent(displayRenderer);
}

void App::OnCleanup() {
    SDL_Quit();
}

