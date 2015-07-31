//
//  Clay.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 31.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Clay.h"

#include "ColorUtils.h"

Clay::Clay(){
}
Clay::~Clay(){
    
    delete drawShader;
    delete transformfeedbackShader;
    
    glDeleteVertexArrays(2, vao);
    glDeleteBuffers(2,vbo);
    glDeleteQueries(1, &query);
}

bool Clay::Init(){
    glGenQueries(1, &query);
    initProgram();
    initVertexArray();
    
    return true;
    
}

void Clay::Update(){
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
    
    

    glm::vec3 g(0.0f, -9.81f, 0.0f);
    static float angle = 0.0f;
    
    // get the time in seconds
    float t = glfwGetTime();
    angle += 100* t;
    
    for (int i = 0; i < spheres; i++) {
        glm::vec3 c = center[i];
        center[i] = glm::rotate(c, angle*t, glm::vec3(-1, 1, 0));
        
    }
    
    
    
    // use the transform shader program
    transformfeedbackShader->use();
    
    // set the uniforms
    glUniform1f(transformfeedbackShader->uniform("t"), t);
    
    // bind the current vao
    glBindVertexArray(vao[(current_buffer+1)%2]);
    
    // bind transform feedback target
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[current_buffer]);
    
    glEnable(GL_RASTERIZER_DISCARD);
    
    // perform transform feedback


    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, num_of_paritcles);
    glEndTransformFeedback();
    
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    GLuint primitives_written;
    glGetQueryObjectuiv( query, GL_QUERY_RESULT, &primitives_written );
    if(primitives_written > 0 )
        fprintf( stderr, "Primitives written to TFB: %d !\n", primitives_written );
    
    
    glDisable(GL_RASTERIZER_DISCARD);
}


void Clay::Render(glm::mat4 view, glm::mat4 projection){
    
    // clear first
    float t = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // use the shader program
    drawShader->use();
    
    // calculate ViewProjection matrix
    
    glm::mat4 model(1);
    glUniformMatrix4fv(drawShader->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(drawShader->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(drawShader->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    // bind the current vao
    glBindVertexArray(vao[current_buffer]);
    
    // draw
    glDrawArrays(GL_POINTS, 0, num_of_paritcles);
    

    current_buffer = (current_buffer + 1) % 2;
}

bool Clay::initProgram(){
    // draw shader
    {
        Shader vertexShader(GL_VERTEX_SHADER);
        vertexShader.loadFromFile("shaders/clay.vert");
        vertexShader.compile();
        
//        Shader geometryShader(GL_GEOMETRY_SHADER);
//        geometryShader.loadFromFile("shaders/clay.geom");
//        geometryShader.compile();
        
        Shader fragmetShader(GL_FRAGMENT_SHADER);
        fragmetShader.loadFromFile("shaders/clay.frag");
        fragmetShader.compile();
        
        drawShader = new ShaderProgram();
        drawShader->attachShader(vertexShader);
//        drawShader->attachShader(geometryShader);
        drawShader->attachShader(fragmetShader);
        drawShader->linkProgram();
        
        drawShader->addUniform("model");
        drawShader->addUniform("view");
        drawShader->addUniform("projection");
        

        
    }
    
    // transform feedback shader
    {
        Shader vertexShader(GL_VERTEX_SHADER);
        vertexShader.loadFromFile("shaders/clay-transform.vert");
        vertexShader.compile();
        
        
//        Shader fragmentShader(GL_FRAGMENT_SHADER);
//        fragmentShader.loadFromString(transform_fragment_source);
//        fragmentShader.compile();
        
        transformfeedbackShader = new ShaderProgram();
        transformfeedbackShader->attachShader(vertexShader);
//        transformfeedbackShader->attachShader(fragmentShader);
        
        
        // specify transform feedback output
        const char *varyings[] = {"outposition", "outvelocity"};
        glTransformFeedbackVaryings(transformfeedbackShader->id(), 2, varyings, GL_INTERLEAVED_ATTRIBS);
        
        transformfeedbackShader->linkProgram();
        
        
        transformfeedbackShader->addAttribute("inposition");
        transformfeedbackShader->addAttribute("invelocity");
        
        transformfeedbackShader->addUniform("t");
        
    }
    return true;
}

bool Clay::initVertexArray(){
    
    
    // randomly place particles in a cube
    std::vector<ClayElement> vertexData(num_of_paritcles);
    std::vector<glm::vec3> colors;
    generateColors(num_of_paritcles, colors);
    
    for(int i = 0;i<num_of_paritcles;++i) {
        
        // initial position
        vertexData[i].position = glm::normalize( glm::vec3(
                                      0.5f-float(std::rand())/RAND_MAX,
                                      0.5f-float(std::rand())/RAND_MAX,
                                      0.5f-float(std::rand())/RAND_MAX
                                      ));
        vertexData[i].position *= 5.0f;
        
        // initial velocity
        vertexData[i].color = colors[i];
    }
    
    
    glGenVertexArrays(2, vao);
    glGenBuffers(2, vbo);
    
    for(int i = 0;i<2;++i) {
        glBindVertexArray(vao[i]);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
        
        // fill with initial data
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
        
        // set up generic attrib pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(transformfeedbackShader->attribute("inposition"),
                              3, GL_FLOAT,
                              GL_FALSE,
                              sizeof(ClayElement), // 6*sizeof(GLfloat),
                              (void*)offsetof(struct ClayElement, position)); //(char*)0 + 0*sizeof(GLfloat));
        // set up generic attrib pointers
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(transformfeedbackShader->attribute("invelocity"),
                              3, GL_FLOAT,
                              GL_FALSE,
                              sizeof(ClayElement), //6*sizeof(GLfloat),
                              (void*)offsetof(struct ClayElement, color)); //(char*)0 + 0*sizeof(GLfloat));(char*)0 + 3*sizeof(GLfloat));
    }
    
    // "unbind" vao
    glBindVertexArray(0);
    
    
    return true;
}