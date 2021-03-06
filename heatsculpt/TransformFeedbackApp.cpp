//
//  TransformFeedbackApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 15.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include <vector>
#include <glm/gtx/rotate_vector.hpp>
#include "TransformFeedbackApp.h"


// the vertex shader simply passes through data
std::string vertex_source =
"#version 330\n"
"layout(location = 0) in vec4 vposition;\n"
"void main() {\n"
"   gl_Position = vposition;\n"
"}\n";

// the geometry shader creates the billboard quads
std::string geometry_source =
"#version 330\n"
"uniform mat4 View;\n"
"uniform mat4 Projection;\n"
"layout (points) in;\n"
"layout (triangle_strip, max_vertices = 4) out;\n"
"out vec2 txcoord;\n"
"void main() {\n"
"   vec4 pos = View*gl_in[0].gl_Position;\n"
"   txcoord = vec2(-1,-1);\n"
"   gl_Position = Projection*(pos+0.2*vec4(txcoord,0,0));\n"
"   EmitVertex();\n"
"   txcoord = vec2( 1,-1);\n"
"   gl_Position = Projection*(pos+0.2*vec4(txcoord,0,0));\n"
"   EmitVertex();\n"
"   txcoord = vec2(-1, 1);\n"
"   gl_Position = Projection*(pos+0.2*vec4(txcoord,0,0));\n"
"   EmitVertex();\n"
"   txcoord = vec2( 1, 1);\n"
"   gl_Position = Projection*(pos+0.2*vec4(txcoord,0,0));\n"
"   EmitVertex();\n"
"}\n";

// the fragment shader creates a bell like radial color distribution
std::string fragment_source =
"#version 330\n"
"in vec2 txcoord;\n"
"layout(location = 0) out vec4 FragColor;\n"
"void main() {\n"
"   float s = 0.2*(1/(1+15.*dot(txcoord, txcoord))-1/16.);\n"
"   FragColor = s*vec4(0.3,0.3,1.0,1);\n"
"}\n";

// the transform feedback shader only has a vertex shader
std::string transform_vertex_source =
"#version 330\n"
"uniform vec3 center[3];\n"
"uniform float radius[3];\n"
"uniform vec3 g;\n"
"uniform float dt;\n"
"uniform float bounce;\n"
"uniform int seed;\n"
"in vec3 inposition;\n"
"in vec3 invelocity;\n"
"out vec3 outposition;\n"
"out vec3 outvelocity;\n"

"float hash(int x) {\n"
"   x = x*1235167 + gl_VertexID*948737 + seed*9284365;\n"
"   x = (x >> 13) ^ x;\n"
"   return ((x * (x * x * 60493 + 19990303) + 1376312589) & 0x7fffffff)/float(0x7fffffff-1);\n"
"}\n"

"void main() {\n"
"   outvelocity = invelocity;\n"
"   for(int j = 0;j<3;++j) {\n"
"       vec3 diff = inposition-center[j];\n"
"       float dist = length(diff);\n"
"       float vdot = dot(diff, invelocity);\n"
"       if(dist<radius[j] && vdot<0.0)\n"
"           outvelocity -= bounce*diff*vdot/(dist*dist);\n"
"   }\n"
"   outvelocity += dt*g;\n"
"   outposition = inposition + dt*outvelocity;\n"
"   if(outposition.y < -30.0)\n"
"   {\n"
"       outvelocity = vec3(0,0,0);\n"
"       outposition = 0.5-vec3(hash(3*gl_VertexID+0),hash(3*gl_VertexID+1),hash(3*gl_VertexID+2));\n"
"       outposition = vec3(0,20,0) + 5.0*outposition;\n"
"   }\n"
"}\n";


std::string transform_fragment_source =
"#version 330\n"
"out vec4 outColor;\n"
"void main() {\n"
"    outColor = vec4(0.8, 0.2, 0.2, 1.0);\n"
"}\n";

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
    
    
    camera.SetPosition(glm::vec3(0.0f, 0.0f, -30.0f));

    
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
        
        
        inposition_location = transformfeedbackShader->addAttribute("inposition");
        invelocity_location = transformfeedbackShader->addAttribute("invelocity");
        
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
                                      -1.5f-float(std::rand())/RAND_MAX,
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
        glVertexAttribPointer(inposition_location, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));
        // set up generic attrib pointers
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(invelocity_location, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (char*)0 + 3*sizeof(GLfloat));
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
    
    static float angle = 0.0f;

    // get the time in seconds
    float t = glfwGetTime();
    angle += 100* t;
    
    for (int i = 0; i < spheres; i++) {
        glm::vec3 c = center[i];
        center[i] = glm::rotate(c, angle, glm::vec3(1, 1, 0));

    }
    
    
    
    // use the transform shader program
    transformfeedbackShader->use();
    
    // set the uniforms
    glUniform3fv(center_location, 3, reinterpret_cast<GLfloat*>(center));
    glUniform1fv(radius_location, 3, reinterpret_cast<GLfloat*>(radius));
    glUniform3fv(g_location, 1, glm::value_ptr(g));
    glUniform1f(dt_location, dt);
    glUniform1f(bounce_location, bounce);
    glUniform1i(seed_location, std::rand());
    
    // bind the current vao
    glBindVertexArray(vao[(current_buffer+1)%buffercount]);
    
    // bind transform feedback target
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[current_buffer]);
    
    glEnable(GL_RASTERIZER_DISCARD);
    
    // perform transform feedback
    GLuint query;
    glGenQueries(1, &query);
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, particles);
    glEndTransformFeedback();
    
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    GLuint primitives_written;
    glGetQueryObjectuiv( query, GL_QUERY_RESULT, &primitives_written );
    if(primitives_written > 0 )
        fprintf( stderr, "Primitives written to TFB: %d !\n", primitives_written );
    glDeleteQueries(1, &query);
    
    glDisable(GL_RASTERIZER_DISCARD);
    
    
}
void TransformFeedbackApp::Render(){
    App::Render();
    

    // clear first
    float t = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // use the shader program
    drawShader->use();
    
    // calculate ViewProjection matrix
    
    
    glUniformMatrix4fv(View_location, 1, GL_FALSE, glm::value_ptr(camera.view));
    glUniformMatrix4fv(Projection_location, 1, GL_FALSE, glm::value_ptr(camera.projection));
    
    // bind the current vao
    glBindVertexArray(vao[current_buffer]);
    
    // draw
    glDrawArrays(GL_POINTS, 0, particles);
    
//    // check for errors
//    GLenum error = glGetError();
//    if(error != GL_NO_ERROR) {
//        std::cerr << error << std::endl;
//    }
    
//    // finally swap buffers
//    glfwSwapBuffers(window);
    
    // advance buffer index
    current_buffer = (current_buffer + 1) % buffercount;
    
}