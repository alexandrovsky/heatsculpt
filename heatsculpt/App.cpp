//
//  App.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 12.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "App.h"

#include <gl/glew.h>

#include "Shader.h"
#include "ShaderProgram.h"


using namespace std;

App* App::_instance = NULL;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        App::_instance->OnKeyDown(window, key, scancode, action, mods);
    }else if (action == GLFW_RELEASE){
        App::_instance->OnKeyUp(window, key, scancode, action, mods);
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    App::_instance->OnMouseMove(xpos, ypos);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (GLFW_PRESS == action) {
        App::_instance->OnMouseDown(button, mods);
    }else if(GLFW_RELEASE == action){
        App::_instance->OnMouseUp(button, mods);
    }
}



App::App(const std::string& window_title, bool fullscreen) {
    
    _instance = this;
    this->window_width = 0;
    this->window_height = 0;
    this->isRunning = true;
    this->isFullScreen = fullscreen;
    this->backgroundColor = glm::vec4(31.0/255.0f, 147./255.0f, 194/255.0f, 1.0f);
}


App::~App(){
}

int App::Start() {
    
    Init();
    this->isRunning = true;
    MainLoop();
    return 0;
}

void App::MainLoop() {
    
    while(isRunning) {
        glfwPollEvents();
        
        Update();
        Render();
        
        if (glfwWindowShouldClose(window)) {
            Exit();
        }
    }
    
    Cleanup();
}

bool App::InitOpenGL(){
    
    const unsigned char* glVersion = glGetString(GL_VERSION);
    const unsigned char* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    std::cout << "gl version:" << glVersion << std::endl;
    std::cout << "glsl version:" << glslVersion << std::endl;
    
    
    
    
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



bool App::Init() {

    if(glfwInit() != true) {
        cout << "failed to init glfw" << endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    //fullscreen
    GLFWmonitor* monitor =  glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    this->window_width = mode->width;
    this->window_height = mode->height;

    this->window = glfwCreateWindow(window_width,
                                    window_height,
                                    window_title.c_str(),
                                    isFullScreen ? monitor : NULL,
                                    NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // Init GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return false;
    }
    
    if (!InitOpenGL()) {
        return false;
    }
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    

    camera.window_width = window_width;
    camera.window_height = window_height;

    camera.Reset();

    return true;
}


void App::OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch (key) {
        case GLFW_KEY_LEFT:
            camera.Move(LEFT);
            break;
        case GLFW_KEY_RIGHT:
            camera.Move(RIGHT);
            break;
        case GLFW_KEY_UP:
            camera.Move(FORWARD);
            break;
        case GLFW_KEY_DOWN:
            camera.Move(BACK);
            break;
        case GLFW_KEY_P:
            drawWireFrame = !drawWireFrame;
            break;
        case GLFW_KEY_R:
            camera.Reset();
            break;
            
        case GLFW_KEY_ESCAPE:
            Exit();
            break;
        
        default:
            break;
    }
}


void App::OnKeyUp(GLFWwindow* window, int key, int scancode, int action, int mods){
}

void App::OnMouseMove(double mX, double mY){
    mouse_cursor.x = mX;
    mouse_cursor.y = mY;
    
    camera.Move2D(mX, mY);
}

void App::OnMouseDown(int mouse_btn, int mod){
    camera.SetPos(mouse_btn, 1, mouse_cursor.x, mouse_cursor.y);
}

void App::OnMouseUp(int mouse_btn, int mode){
    camera.SetPos(mouse_btn, 0, mouse_cursor.x, mouse_cursor.y);
}

void App::Exit(){
    isRunning = false;
    glfwDestroyWindow(window);
}



void App::Update() {
    camera.Update();
}

void App::Render() {
    
    glfwSwapBuffers(window);

    
    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );
    
    /* Set the background black */
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
//    glClearColor( 0.0f, 0.15f, 0.3f, 0.0f );
    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();
    
    if (drawWireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

void App::Cleanup() {
    glfwTerminate();
}

