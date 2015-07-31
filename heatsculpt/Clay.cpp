//
//  Clay.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 31.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "Clay.h"



bool Clay::initProgram(){
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
}

bool Clay::initVertexArray(){
}