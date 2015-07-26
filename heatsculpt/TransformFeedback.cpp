//
//  TransformFeedback.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 24.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//
#include <gl/glew.h>
#include <glm/vec3.hpp>
#include "MeshUtils.h"
#include "TransformFeedback.h"
#include "Debug.h"

TransformFeedback::TransformFeedback():modelMatrix(1){
}

TransformFeedback::~TransformFeedback(){
    delete shaderProgram;
    
    glDeleteQueries(1, &query);
    
}

bool TransformFeedback::Init(){
    InitShader();
    InitTransformFeedback();
    return true;
}

bool TransformFeedback::InitShader(){
    
    
    
    shaderProgram = new ShaderProgram();
    Shader vert(GL_VERTEX_SHADER);
    vert.loadFromFile("shaders/tfSimple.vert");
    vert.compile();
    
    
    Shader geom(GL_GEOMETRY_SHADER);
    geom.loadFromFile("shaders/tfSimple.geom");
    geom.compile();
    
    
//    Shader frag(GL_FRAGMENT_SHADER);
//    frag.loadFromFile("shaders/tfSimple.frag");
//    frag.compile();
    
    shaderProgram->attachShader(vert);
//    shaderProgram->attachShader(frag);
    shaderProgram->attachShader(geom);
    
    // specify transform feedback output
    const char *varyings[] = {"outPosition"};
    glTransformFeedbackVaryings(this->shaderProgram->id(), 1, varyings, GL_INTERLEAVED_ATTRIBS);
    PrintProgramInfoLog(shaderProgram->id());
    CheckGlErrors();
    shaderProgram->linkProgram();
    PrintProgramInfoLog(shaderProgram->id());
    


    
    positionAttrib.name = "position";
    positionAttrib.num_of_components = 2;
    positionAttrib.data_type = GL_FLOAT;
    positionAttrib.buffer_type = GL_ARRAY_BUFFER;
    positionAttrib.draw_type = GL_DYNAMIC_DRAW;

    positionAttrib.id = shaderProgram->addAttribute(positionAttrib.name);
    
    return true;
}


bool TransformFeedback::InitTransformFeedback(){
    
    
    
    

    for (int i = 0; i < 1000; i++) {
        float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        vertices.push_back(vec2(x, y));
    }
    
    drawCount = (GLsizei)vertices.size();
    positionAttrib.bytes = sizeof(vertices[0]) * vertices.size();
    
    
    glGenBuffers(1, &source_vbo);
    
    glGenBuffers(1, &destination_vbo);
    glBindBuffer(positionAttrib.buffer_type, destination_vbo);
    glBufferData(positionAttrib.buffer_type, positionAttrib.bytes, vertices.data(), positionAttrib.draw_type);
    
    
    
    glGenTransformFeedbacks(1, &tfo);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, destination_vbo);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    
    
    
    glGenQueries(1,&query);
    
    return true;
}

void TransformFeedback::Update(){
    
    
    glEnable(GL_RASTERIZER_DISCARD);
    
    shaderProgram->use();
//    PrintProgramInfoLog(shaderProgram->id());
    // set uniforms:
    {
    }

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo);
    glBeginTransformFeedback(GL_POINTS);{
        
        glBindBuffer(GL_ARRAY_BUFFER, source_vbo);
        glBufferData(positionAttrib.buffer_type, positionAttrib.bytes, vertices.data(), positionAttrib.draw_type); //GL_STREAM_DRAW
        
        glEnableVertexAttribArray(positionAttrib.id);
        glVertexAttribPointer(positionAttrib.id, positionAttrib.num_of_components, positionAttrib.data_type, false, 0, 0);
        
        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
        glDrawArrays(GL_POINTS, 0, drawCount);
        glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
//        CheckGlErrors();
        GLuint primitives_written;
        glGetQueryObjectuiv( query, GL_QUERY_RESULT, &primitives_written );
        if(primitives_written > 0 )
            fprintf( stderr, "Primitives written to TFB: %d !\n", primitives_written );

        glDisableVertexAttribArray(positionAttrib.id);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        
    }glEndTransformFeedback();
    
    shaderProgram->disable();
    
    glDisable(GL_RASTERIZER_DISCARD);
    
//    // bind the current vao
//
//    glBindBuffer(GL_ARRAY_BUFFER, source_vbo);
//    
//    PrintProgramInfoLog(shaderProgram->id());
//    
//    // bind transform feedback target
//    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, destination_vbo);
//    
//    PrintProgramInfoLog(shaderProgram->id());
//    
//    CheckGlErrors();
    
    
    

//    PrintProgramInfoLog(shaderProgram->id());
//    CheckGlErrors();
//    
//    
//    
//    glDrawArrays(GL_POINTS, 0, this->drawCount);
//    CheckGlErrors();
//    PrintProgramInfoLog(shaderProgram->id());
//    glEndTransformFeedback();
//    
//    CheckGlErrors();
//    PrintProgramInfoLog(shaderProgram->id());
//    
//    glEndQuery( GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN );
//    
//    CheckGlErrors();
//    
//    GLuint primitives_written;
//    // read back query results
//    glGetQueryObjectuiv( query, GL_QUERY_RESULT, &primitives_written );
//    fprintf( stderr, "Primitives written to TFB: %d !\n", primitives_written );
//    
//    
//    glDisable(GL_RASTERIZER_DISCARD);
//    
//    shaderProgram->disable();
//    
//    std::swap(source_vao, destination_vao);
//    std::swap(source_vbo, destination_vbo);
//    
//    //    GetFirstNMessages(1);
//    CheckGlErrors();

    
}

void TransformFeedback::Draw(){
    //Bind the outputVBO just like any other VBO
    glBindBuffer(GL_ARRAY_BUFFER, destination_vbo);
    
    //We're using the fixed functionality pipeline here,
    //so just set it up to read positions from the outputVBO.
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(positionAttrib.num_of_components, positionAttrib.data_type, 0, 0);
    
    glPointSize(5.0);
    glColor3f(1.0f, 0.0f, 0.0f);
    //glDrawTransformFeedback is a special draw call that is very similar to
    //glDrawArrays(). It allows us to draw all points that were output by our
    //shader without having to involve the CPU to determine how many they were.
    //This is the same as glDrawArrays(GL_POINTS, 0, num_points_that_passed);,
    //but is a LOT faster than if we had used a query object to get the count
    //and then manually calling glDrawArrays() with that count.
    glDrawTransformFeedback(GL_POINTS, tfo);
    
    //Clean up...
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
template<typename T> void TransformFeedback::addBuffer(GLuint& vbo, vector<T> data, Attribute& attribute){
    
    shaderProgram->use();
    
    attribute.id = shaderProgram->addAttribute(attribute.name);
    attribute.bytes = sizeof(T) * data.size();
    
    
    glBindBuffer(attribute.buffer_type, vbo);
    glBufferData(attribute.buffer_type, attribute.bytes, data.data(), attribute.draw_type);
    
    
    glEnableVertexAttribArray(attribute.id);
    glVertexAttribPointer(attribute.id, attribute.num_of_components, attribute.data_type, GL_FALSE, 0, 0);
    
    glBindVertexArray(0);
    shaderProgram->disable();
}
