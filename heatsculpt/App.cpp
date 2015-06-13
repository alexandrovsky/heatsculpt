//
//  App.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 12.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "App.h"


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


Shader* vertexShader;
Shader* geometryShader;
Shader* fragmentShader;

ShaderProgram* shaderProgram;

string vertexsource, fragmentsource;
GLuint vbo;


App::App(const std::string& window_title, int width, int height) {
    
    _instance = this;
    this->window_width = width;
    this->window_height = height;
    this->isRunning = true;
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    

    this->window = glfwCreateWindow(window_width, window_height, window_title.c_str(), NULL, NULL);
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
    
    camera.SetMode(FREE);	//Two Modes FREE and ORTHO
    camera.SetPosition(glm::vec3(0, 0, -10));
    camera.SetViewport(0, 0, window_width, window_height);
    camera.SetLookAt(glm::vec3(0, 0, 0));
    camera.SetClipping(.01, 1000);
    camera.SetFOV(45);
    
    
    // setup shader
    //vertex:
    const char* vertexShaderSrc =
                        GLSL(
                             in vec2 pos;
                             in vec3 color;
                             in float sides;
                             
                             out vec3 vColor;
                             out float vSides;
                             
                             void main() {
                                 gl_Position = vec4(pos, 0.0, 1.0);
                                 vColor = color;
                                 vSides = sides;
                             }
                            );
    
    vertexShader = new Shader(GL_VERTEX_SHADER);
    vertexShader->loadFromString(vertexShaderSrc);
    vertexShader->compile();

    
    // fragment:
    const char* fragmentShaderSrc =
    GLSL(
         out vec4 outColor;
         
         void main() {
             outColor = vec4(1.0, 0.0, 0.0, 1.0);
         }
    );
    
    
    fragmentShader = new Shader(GL_FRAGMENT_SHADER);
    fragmentShader->loadFromString(fragmentShaderSrc);
    fragmentShader->compile();
    
    
    // geometry
    const char* geometryShaderSrc =
    GLSL(
         layout(points) in;
         layout(line_strip, max_vertices = 64) out;
         
         in vec3 vColor[];
         in float vSides[];
         
         out vec3 fColor;
         
         const float PI = 3.1415926;
         
         void main() {
             fColor = vColor[0];
             
             for (int i = 0; i <= vSides[0]; i++) {
                 // Angle between each side in radians
                 float ang = PI * 2.0 / vSides[0] * i;
                 
                 // Offset from center of point (0.3 to accomodate for aspect ratio)
                 vec4 offset = vec4(cos(ang) * 0.3, -sin(ang) * 0.4, 0.0, 0.0);
                 gl_Position = gl_in[0].gl_Position + offset;
                 
                 EmitVertex();
             }
             
             EndPrimitive();
         }
         );
    
    
    geometryShader = new Shader(GL_GEOMETRY_SHADER_ARB);
    geometryShader->loadFromString(geometryShaderSrc);
    geometryShader->compile();
    
    
    
    
    
    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader(*vertexShader);
    shaderProgram->attachShader(*fragmentShader);
    shaderProgram->attachShader(*geometryShader);
    shaderProgram->linkProgram();
    shaderProgram->use();
    
    
    glGenBuffers(1, &vbo);
    
    float points[] = {
        //  Coordinates  Color             Sides
        -0.45f,  0.45f, 1.0f, 0.0f, 0.0f,  4.0f,
        0.45f,  0.45f, 0.0f, 1.0f, 0.0f,  8.0f,
        0.45f, -0.45f, 0.0f, 0.0f, 1.0f, 16.0f,
        -0.45f, -0.45f, 1.0f, 1.0f, 0.0f, 32.0f
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    
    
    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Specify layout of point data
    GLint posAttrib= shaderProgram->addAttribute("pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), 0);
    
    GLint colAttrib = shaderProgram->addAttribute("color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void*) (2 * sizeof(float)));
    
    GLint sidesAttrib = shaderProgram->addAttribute("sides");
    glEnableVertexAttribArray(sidesAttrib);
    glVertexAttribPointer(sidesAttrib, 1, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void*) (5 * sizeof(float)));
    
    
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
}



void App::Update() {
    camera.Update();
}

void App::Render() {
    
    glfwSwapBuffers(window);

    
    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );
    
    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();


    glDrawArrays(GL_POINTS, 0, 4);
    
    
//    glBegin( GL_QUADS );                /* Draw A Quad */
//    glVertex3f( -1.0f,  1.0f, 0.0f ); /* Top Left */
//    glVertex3f(  1.0f,  1.0f, 0.0f ); /* Top Right */
//    glVertex3f(  1.0f, -1.0f, 0.0f ); /* Bottom Right */
//    glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
//    glEnd( );                           /* Done Drawing The Quad */
    

}

void App::Cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

