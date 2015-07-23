//
//  TransformFeedbackApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 15.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include <vector>

#include "TransformFeedbackApp.h"


// the vertex shader simply passes through data
std::string vertex_source =
GLSL(
layout(location = 0) in vec4 vposition;
void main() {
   gl_Position = vposition;
});

// the geometry shader creates the billboard quads



std::string geometry_source = GLSL(
                                   uniform mat4 View;
                                   uniform mat4 Projection;
                                   layout (points) in;
                                   layout (triangle_strip, max_vertices = 4) out;
                                   out vec2 txcoord;
                                   void main() {
                                       vec4 pos = View*gl_in[0].gl_Position;
                                       txcoord = vec2(-1,-1);
                                       gl_Position = Projection*(pos+0.2*vec4(txcoord,0,0));
                                       EmitVertex();
                                       txcoord = vec2( 1,-1);
                                       gl_Position = Projection*(pos+0.2*vec4(txcoord,0,0));
                                       EmitVertex();
                                       txcoord = vec2(-1, 1);
                                       gl_Position = Projection*(pos+0.2*vec4(txcoord,0,0));
                                       EmitVertex();
                                       txcoord = vec2( 1, 1);
                                       gl_Position = Projection*(pos+0.2*vec4(txcoord,0,0));
                                       EmitVertex();
                                   });

// the fragment shader creates a bell like radial color distribution
std::string fragment_source =
GLSL(
in vec2 txcoord;
layout(location = 0) out vec4 FragColor;
void main() {
   float s = 0.2*(1/(1+15.*dot(txcoord, txcoord))-1/16.);
   FragColor = s*vec4(0.3,0.3,1.0,1);
});

// the transform feedback shader only has a vertex shader
std::string transform_vertex_source =
GLSL(
uniform vec3 center[3];
uniform float radius[3];
uniform vec3 g;
uniform float dt;
uniform float bounce;
uniform int seed;
layout(location = 0) in vec3 inposition;
layout(location = 1) in vec3 invelocity;
out vec3 outposition;
out vec3 outvelocity;

float hash(int x) {
   x = x*1235167 + gl_VertexID*948737 + seed*9284365;
   x = (x >> 13) ^ x;
   return ((x * (x * x * 60493 + 19990303) + 1376312589) & 0x7fffffff)/float(0x7fffffff-1);
}

void main() {
   outvelocity = invelocity;
   for(int j = 0;j<3;++j) {
       vec3 diff = inposition-center[j];
       float dist = length(diff);
       float vdot = dot(diff, invelocity);
       if(dist<radius[j] && vdot<0.0)
           outvelocity -= bounce*diff*vdot/(dist*dist);
   }
   outvelocity += dt*g;
   outposition = inposition + dt*outvelocity;
   if(outposition.y < -30.0)
   {
       outvelocity = vec3(0,0,0);
       outposition = 0.5-vec3(hash(3*gl_VertexID+0),hash(3*gl_VertexID+1),hash(3*gl_VertexID+2));
       outposition = vec3(0,20,0) + 5.0*outposition;
   }
});


std::string transform_fragment_source =
GLSL(
out vec4 outColor;
void main() {
    outColor = vec4(0.8, 0.2, 0.2, 1.0);
});

TransformFeedbackApp::TransformFeedbackApp(const std::string& window_title, bool fullscreen):App::App(window_title, fullscreen){
}

TransformFeedbackApp::~TransformFeedbackApp(){
    delete drawShader;
    delete transformfeedbackShader;
}


bool TransformFeedbackApp::Init(){
    
    bool res = App::Init();
    if (!res) {
        return res;
    }
    
    // draw shader
    {
        Shader vertexShader(GL_VERTEX_SHADER);
        vertexShader.loadFromString(vertex_source);
        vertexShader.compile();
        
        Shader geometryShader(GL_GEOMETRY_SHADER);
        geometryShader.loadFromString(geometry_source);
        geometryShader.compile();
        
        Shader fragmetShader(GL_FRAGMENT_SHADER);
        fragmetShader.loadFromString(fragment_source);
        fragmetShader.compile();
        
        drawShader = new ShaderProgram();
        drawShader->attachShader(vertexShader);
        drawShader->attachShader(geometryShader);
        drawShader->attachShader(fragmetShader);
        drawShader->linkProgram();
        
        
        View_location = drawShader->addUniform("View");
        Projection_location = drawShader->addUniform("Projection");
        
    }
    
    // transform feedback shader
    {
        Shader vertexShader(GL_VERTEX_SHADER);
        vertexShader.loadFromString(transform_vertex_source);
        vertexShader.compile();
        
        
        Shader fragmentShader(GL_FRAGMENT_SHADER);
        fragmentShader.loadFromString(transform_fragment_source);
        fragmentShader.compile();
        
        transformfeedbackShader = new ShaderProgram();
        transformfeedbackShader->attachShader(vertexShader);
        transformfeedbackShader->attachShader(fragmentShader);
        
        
        // specify transform feedback output
        const char *varyings[] = {"outposition", "outvelocity"};
        glTransformFeedbackVaryings(transformfeedbackShader->id(), 2, varyings, GL_INTERLEAVED_ATTRIBS);
        
        transformfeedbackShader->linkProgram();
        
        
        center_location = transformfeedbackShader->addUniform("center");
        radius_location = transformfeedbackShader->addUniform("radius");
        g_location = transformfeedbackShader->addUniform("g");
        dt_location = transformfeedbackShader->addUniform("dt");
        bounce_location = transformfeedbackShader->addUniform("bounce");
        seed_location = transformfeedbackShader->addUniform("seed");
        
    }
    
    // randomly place particles in a cube
    std::vector<glm::vec3> vertexData(2*particles);
    for(int i = 0;i<particles;++i) {
        // initial position
        vertexData[2*i+0] = glm::vec3(
                                      0.5f-float(std::rand())/RAND_MAX,
                                      0.5f-float(std::rand())/RAND_MAX,
                                      0.5f-float(std::rand())/RAND_MAX
                                      );
        vertexData[2*i+0] = glm::vec3(0.0f,20.0f,0.0f) + 5.0f*vertexData[2*i+0];
        
        // initial velocity
        vertexData[2*i+1] = glm::vec3(0,0,0);
    }
    

    glGenVertexArrays(buffercount, vao);
    glGenBuffers(buffercount, vbo);
    
    for(int i = 0;i<buffercount;++i) {
        glBindVertexArray(vao[i]);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
        
        // fill with initial data
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
        
        // set up generic attrib pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));
        // set up generic attrib pointers
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (char*)0 + 3*sizeof(GLfloat));
    }
    
    // "unbind" vao
    glBindVertexArray(0);
    
    // we ar blending so no depth testing
    glDisable(GL_DEPTH_TEST);
    
    // enable blending
    glEnable(GL_BLEND);
    //  and set the blend function to result = 1*source + 1*destination
    glBlendFunc(GL_ONE, GL_ONE);
    
    
    
    
    return true;
}
void TransformFeedbackApp::Update(){
    App::Update();
    
    // define spheres for the particles to bounce off
    const int spheres = 3;
    glm::vec3 center[spheres];
    float radius[spheres];
    center[0] = glm::vec3(0,12,1);
    radius[0] = 3;
    center[1] = glm::vec3(-3,0,0);
    radius[1] = 7;
    center[2] = glm::vec3(5,-10,0);
    radius[2] = 12;
    
    
    float dt = 1.0f/60.0f;
    glm::vec3 g(0.0f, -9.81f, 0.0f);
    float bounce = 1.2f; // inelastic: 1.0f, elastic: 2.0f
    
    
    
    // get the time in seconds
    float t = glfwGetTime();
    
    
    
    
    // use the transform shader program
    transformfeedbackShader->use();
    
    // set the uniforms
    glUniform3fv(center_location, 3, reinterpret_cast<GLfloat*>(center));
    glUniform1fv(radius_location, 3, reinterpret_cast<GLfloat*>(radius));
    glUniform3fv(g_location, 1, glm::value_ptr(g));
    glUniform1f(dt_location, dt);
    glUniform1f(bounce_location, bounce);
    //    glUniform1i(seed_location, std::rand());
    glUniform1i(seed_location, 0);
    
    // bind the current vao
    glBindVertexArray(vao[(current_buffer+1)%buffercount]);
    
    // bind transform feedback target
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[current_buffer]);
    
    glEnable(GL_RASTERIZER_DISCARD);
    
    // perform transform feedback
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, particles);
    glEndTransformFeedback();
    
    glDisable(GL_RASTERIZER_DISCARD);
}
void TransformFeedbackApp::Render(){
    App::Render();
    
    // clear first
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // use the shader program
    drawShader->use();
    
    // calculate ViewProjection matrix
    glm::mat4 Projection = glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 100.f);
    
    // translate the world/view position
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -30.0f));
    
    // make the camera rotate around the origin
//    View = glm::rotate(View, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//    View = glm::rotate(View, -22.5f*t, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // set the uniform
    glUniformMatrix4fv(View_location, 1, GL_FALSE, glm::value_ptr(View));
    glUniformMatrix4fv(Projection_location, 1, GL_FALSE, glm::value_ptr(Projection));
    
    // bind the current vao
    glBindVertexArray(vao[current_buffer]);
    
    // draw
    glDrawArrays(GL_POINTS, 0, particles);
    
    // advance buffer index
    current_buffer = (current_buffer + 1) % buffercount;
    
}