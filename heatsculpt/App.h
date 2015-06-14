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
#include <string>
#include <glm/glm.hpp>

#include <GLEW/GLEW.h>

#include "GLFW/glfw3.h"
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>


#include "Camera.h"
#include "Mesh.h"

class App {
protected:
    
    glm::vec2 mouse_cursor;
    
    bool isRunning;
    
    std::string window_title;
    int window_width;
    int window_height;

    GLFWwindow* window;
    
    Camera camera;
    Mesh* mesh;
    virtual void MainLoop();
    
public:
    static App* _instance;
    
    App(const std::string& window_title, int window_width, int window_height);
    virtual ~App();
    int OnExecute();

    bool Init();
    
    int Start();
    
    
    virtual void Update();
    virtual void Render();
    virtual void Cleanup();
    virtual void Exit();
    
    
    
    virtual void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods);
    virtual void OnKeyUp(GLFWwindow* window, int key, int scancode, int action, int mods);
    virtual void OnMouseMove(double mX, double mY);
    virtual void OnMouseDown(int mouse_btn,int mod);
    virtual void OnMouseUp(int mouse_btn, int mod);

    

    
    
private:
    bool InitOpenGL();

};

#endif /* defined(__heatsculpt__CApp__) */
