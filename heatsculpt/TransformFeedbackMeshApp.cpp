//
//  TransformFeedbackMeshApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 24.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "TransformFeedbackMeshApp.h"
#include "ColorUtils.h"
TransformFeedbackMeshApp::TransformFeedbackMeshApp(const std::string& window_title, bool fullscreen):App::App(window_title, fullscreen){

}


TransformFeedbackMeshApp::~TransformFeedbackMeshApp(){
    delete transformFeedback;
}



bool TransformFeedbackMeshApp::Init(){
    bool res = App::Init();
    
    
    //camera.SetPosition(glm::vec3(0.0f, 0.0f, -30.0f));
    
    if (!res) {
        return false;
    }
    
    transformFeedback = new TransformFeedback();
    
    vector<string> varyings;
    varyings.push_back("vPosition");
    //varyings.push_back("vColor");
    
    vector<pair<string, GLenum>> shaderSources;
    shaderSources.push_back(pair<string, GLenum>("shaders/tfSimple.vert", GL_VERTEX_SHADER));
    shaderSources.push_back(pair<string, GLenum>("shaders/tfSimple.frag", GL_FRAGMENT_SHADER));
    
    transformFeedback->InitShader(shaderSources, varyings);
    // position
    {
        TransformFeedbackAttribute* positionAttrib = new TransformFeedbackAttribute();
        positionAttrib->name = "inPosition";
        positionAttrib->num_of_components = 3;
        positionAttrib->data_type = GL_FLOAT;
        positionAttrib->buffer_type = GL_ARRAY_BUFFER;
        positionAttrib->draw_type = GL_DYNAMIC_DRAW;
        
        positionAttrib->id = transformFeedback->shaderProgram->addAttribute(positionAttrib->name);
        transformFeedback->varyingAttributes.push_back(positionAttrib);
        
        
        vector<glm::vec3> vertices;
        for (int i = 0; i < 100000; i++) {
            float x = 0.5f - static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float y = 0.5f - static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float z = 0.5f - static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            vertices.push_back(glm::normalize(glm::vec3(x, y, z)) );
        }
        
        transformFeedback->SetDrawCount((GLsizei)vertices.size());
        transformFeedback->AddBuffer(vertices, positionAttrib);
    }
    

    // color
    {
        TransformFeedbackAttribute* colorAttrib = new TransformFeedbackAttribute();
        colorAttrib->name = "inColor";
        colorAttrib->num_of_components = 3;
        colorAttrib->data_type = GL_FLOAT;
        colorAttrib->buffer_type = GL_ARRAY_BUFFER;
        colorAttrib->draw_type = GL_DYNAMIC_DRAW;
        
        colorAttrib->id = transformFeedback->shaderProgram->addAttribute(colorAttrib->name);
        //transformFeedback->varyingAttributes.push_back(colorAttrib);
        
        
        vector<glm::vec3> colors;
        generateSingleColor(transformFeedback->drawCount, colors, vec3(0, 1, 0));
        transformFeedback->AddBuffer(colors, colorAttrib);

    }
    
    // draw shader;
    {
        drawShader = new ShaderProgram();
        Shader vert(GL_VERTEX_SHADER);
        vert.loadFromFile("shaders/simple.vert");
        vert.compile();
        
        Shader frag(GL_FRAGMENT_SHADER);
        frag.loadFromFile("shaders/simple.frag");
        frag.compile();
        
        drawShader->attachShader(vert);
        drawShader->attachShader(frag);
        
        drawShader->linkProgram();
        
        
        drawShader->use();
        
        drawShader->addAttribute("position");
        drawShader->addAttribute("color");
        
        
        GLuint model = drawShader->addUniform("model");
        glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(transformFeedback->modelMatrix));
        GLuint view = drawShader->addUniform("view");
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera.view));
        GLuint projection = drawShader->addUniform("projection");
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
    
        drawShader->disable();
    }
    
    
    
    
    return true;
}
void TransformFeedbackMeshApp::Update(){
    App::Update();
    transformFeedback->Update();
}
void TransformFeedbackMeshApp::Render(){
    App::Render();
    
    
    
    
//    drawShader->use();
//    
//    
//    
//    
//    GLuint m = drawShader->uniform("model");
//    glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(transformFeedback->modelMatrix));
//    
//    GLuint v = drawShader->uniform("view");
//    glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(camera.view));
//    
//    GLuint p = drawShader->uniform("projection");
//    glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(camera.projection));
//    
//    
////    glBindVertexArray(transformFeedback->vao);
////    glDrawArrays(GL_POINTS, 0, transformFeedback->drawCount);
////    
//    
////    transformFeedback->Draw();
//    drawShader->disable();
    
}








