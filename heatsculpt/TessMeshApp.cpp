//
//  TessMeshApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 07.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "TessMeshApp.h"

#include "MeshUtils.h"
#include "ColorUtils.h"


TessMeshApp::TessMeshApp(const std::string& window_title, bool fullscreen):
App::App(window_title, fullscreen)
,mesh(NULL)
,mouse(NULL)
,meshShaderProgram(NULL)
,mouseShaderProgram(NULL)
{
    
}



TessMeshApp::~TessMeshApp()
{
    delete meshShaderProgram;
    delete mouseShaderProgram;
    delete mesh;
}


void TessMeshApp::initMeshShader(){
    
    Shader vertexShader(GL_VERTEX_SHADER);
    vertexShader.loadFromFile("shaders/tess.vert");
    vertexShader.compile();
    
    Shader fragmentShader(GL_FRAGMENT_SHADER);
    fragmentShader.loadFromFile("shaders/tess.frag");
    fragmentShader.compile();
    
    Shader geometryShader(GL_GEOMETRY_SHADER);
    geometryShader.loadFromFile("shaders/tess.geom");
    geometryShader.compile();
    
    Shader tesselationControlShader(GL_TESS_CONTROL_SHADER);
    tesselationControlShader.loadFromFile("shaders/tess.tcs");
    tesselationControlShader.compile();
    
    Shader tesselationEvaluationShader(GL_TESS_EVALUATION_SHADER);
    tesselationEvaluationShader.loadFromFile("shaders/tess.tes");
    tesselationEvaluationShader.compile();
    
    meshShaderProgram = new ShaderProgram();
    meshShaderProgram->attachShader(vertexShader);
    meshShaderProgram->attachShader(fragmentShader);
    meshShaderProgram->attachShader(geometryShader);
    meshShaderProgram->attachShader(tesselationControlShader);
    meshShaderProgram->attachShader(tesselationEvaluationShader);
    meshShaderProgram->linkProgram();
}


void TessMeshApp::initMesh(){
    
    if (mesh) {
        delete mesh;
    }
    mesh = new Mesh();
    
    vector<vec3> vertices;
    vector<vec3> colors;
    vector<GLuint> indices;
    
    
    
    //    createTetraedron(vertices, indices);
    createIcosphere(vertices, indices);
    
    
    unsigned int num_of_colors = (unsigned int)vertices.size();
    
    colors.reserve(num_of_colors);
    //    generateColors(num_of_colors, colors);
    generateSingleColor(num_of_colors, colors, vec3(1.0f, 1.0f, 1.0f));
    

    // position:
    {
        Attribute positionAttrib;
        positionAttrib.name = "position";
        positionAttrib.num_of_components = 3;
        positionAttrib.data_type = GL_FLOAT;
        positionAttrib.buffer_type = GL_ARRAY_BUFFER;
        
        
        
        mesh->addVBO(vertices, positionAttrib);
        meshShaderProgram->use();
        positionAttrib.id = meshShaderProgram->addAttribute(positionAttrib.name);
        glEnableVertexAttribArray(positionAttrib.id);
        glVertexAttribPointer(positionAttrib.id, positionAttrib.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
        meshShaderProgram->disable();
        mesh->attributes.push_back(positionAttrib);
    }
    
    // color:
    {
        Attribute colorAttrib;
        colorAttrib.name = "color";
        colorAttrib.num_of_components = 3;
        colorAttrib.data_type = GL_FLOAT;
        colorAttrib.buffer_type = GL_ARRAY_BUFFER;
        
        
        
        mesh->addVBO(colors, colorAttrib);
        meshShaderProgram->use();
        colorAttrib.id = meshShaderProgram->addAttribute(colorAttrib.name);
        glEnableVertexAttribArray(colorAttrib.id);
        glVertexAttribPointer(colorAttrib.id, colorAttrib.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
        meshShaderProgram->disable();
        mesh->attributes.push_back(colorAttrib);
    }
    
    
    
    // indices:
    {
        mesh->addIndices(indices);
    }
    
    
    
    // uniforms:
    {
        meshShaderProgram->use();
        GLuint model = meshShaderProgram->addUniform("model");
        glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix));
        GLuint view = meshShaderProgram->addUniform("view");
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera.view));
        GLuint projection = meshShaderProgram->addUniform("projection");
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
        
        
        GLuint light0 = meshShaderProgram->addUniform("lightPosition");
        vec3 lightPosition(0.25f, 0.25f, 1.0f);
        glUniform3fv(light0, 1, glm::value_ptr(lightPosition));
        
        
        GLuint ambient = meshShaderProgram->addUniform("ambientMaterial");
        vec3 ambientMaterial(0.04f, 0.04f, 0.04f);
        glUniform3fv(ambient, 1, glm::value_ptr(ambientMaterial));
        
        
        GLuint diffuse = meshShaderProgram->addUniform("diffuseMaterial");
        vec3 diffuseMaterial(0.0f, 0.75f, 0.75f);
        glUniform3fv(diffuse, 1, glm::value_ptr(diffuseMaterial));
        
        meshShaderProgram->disable();
        
    }
    
}

bool TessMeshApp::Init(){
    
    bool res = App::Init();
    if (!res) { return res; }
    
    
    
    initMeshShader();
    
    meshShaderProgram->use();
    
    initMesh();
    //mouse:
    {
        Shader vertexShader(GL_VERTEX_SHADER);
        vertexShader.loadFromFile("shaders/mouse.vert");
        vertexShader.compile();
        
        Shader fragmentShader(GL_FRAGMENT_SHADER);
        fragmentShader.loadFromFile("shaders/mouse.frag");
        fragmentShader.compile();
        
//        Shader geometryShader(GL_GEOMETRY_SHADER);
//        geometryShader.loadFromFile("shaders/mouse.geom");
//        geometryShader.compile();
        
        
        mouseShaderProgram = new ShaderProgram();
        mouseShaderProgram->attachShader(vertexShader);
        mouseShaderProgram->attachShader(fragmentShader);
//        mouseShaderProgram->attachShader(geometryShader);

        mouseShaderProgram->linkProgram();
        
        
        mouse = new Mesh();
        vector<vec3> vertices;
        vertices.push_back(vec3(0.0f, 0.0f, -5.0f));
        
        vector<vec3> colors;
        colors.push_back(vec3(1.0f, 0.0f, 0.0f));
        
        vector<GLuint> indices;
        indices.push_back(0);
        mouse->addIndices(indices);
        
        

        

        // position
        {
            Attribute positionAttrib;
            positionAttrib.name = "position";
            positionAttrib.num_of_components = 3;
            positionAttrib.data_type = GL_FLOAT;
            positionAttrib.buffer_type = GL_ARRAY_BUFFER;
            
            
            
            mouse->addVBO(vertices, positionAttrib);
            mouseShaderProgram->use();
            positionAttrib.id = mouseShaderProgram->addAttribute(positionAttrib.name);
            glEnableVertexAttribArray(positionAttrib.id);
            glVertexAttribPointer(positionAttrib.id, positionAttrib.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
            mouseShaderProgram->disable();
            mouse->attributes.push_back(positionAttrib);
        }
        

        // color:
        {
            Attribute colorAttrib;
            colorAttrib.name = "color";
            colorAttrib.num_of_components = 3;
            colorAttrib.data_type = GL_FLOAT;
            colorAttrib.buffer_type = GL_ARRAY_BUFFER;
            
            
            
            mouse->addVBO(colors, colorAttrib);
            mouseShaderProgram->use();
            colorAttrib.id = mouseShaderProgram->addAttribute(colorAttrib.name);
            glEnableVertexAttribArray(colorAttrib.id);
            glVertexAttribPointer(colorAttrib.id, colorAttrib.num_of_components, GL_FLOAT, GL_FALSE, 0, 0);
            mouseShaderProgram->disable();
            mouse->attributes.push_back(colorAttrib);
        }

        
        // uniforms:
        {
            mouseShaderProgram->use();
            GLuint model = mouseShaderProgram->addUniform("model");
            glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix));
            GLuint view = mouseShaderProgram->addUniform("view");
            glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera.view));
            GLuint projection = mouseShaderProgram->addUniform("projection");
            glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
            
            GLuint mousePosition = mouseShaderProgram->addUniform("mousePosition");
            vec3 mouse_pos(1.0f, 0.0f, -5.0f);
            glUniform3fv(mousePosition, 1, glm::value_ptr(mouse_pos));
            
            
            mouseShaderProgram->disable();
            
        }
        
    }
    return res;
}

void TessMeshApp::Update(){
    App::Update();
    
    
    mesh->Update();
    
    meshShaderProgram->use();
    
    GLuint m = meshShaderProgram->uniform("model");
    glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix));
    
    GLuint v = meshShaderProgram->uniform("view");
    glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(camera.view));
    
    GLuint p = meshShaderProgram->uniform("projection");
    glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(camera.projection));
    
    
    meshShaderProgram->disable();
    
    
    mouseShaderProgram->use();
    
    m = mouseShaderProgram->uniform("model");
    glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(mesh->modelMatrix));
    
    v = mouseShaderProgram->uniform("view");
    glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(camera.view));
    
    p = mouseShaderProgram->uniform("projection");
    glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(camera.projection));
    


    // cursor
    {
        vec2 mouse = mouse_cursor;
        
        
        
        vec3 view = normalize( camera.camera_look_at - camera.camera_position );
        
        vec3 h = normalize(cross(view, camera.camera_up));
        
        vec3 v = normalize(cross(h, view));
        float rad = radians(camera.field_of_view);
        
        float vLength = tanf(rad /2.0f) * camera.near_clip;
        float hLength = vLength * camera.aspect;
        
        v *= vLength;
        h *= hLength;
        
        mouse.x -= window_width/2;
        mouse.y = window_height - mouse.y - window_height/2;
        
        mouse.x /= window_width/2;
        mouse.y /= window_height/2;
        
        vec3 pos = camera.camera_position + view * (float)camera.near_clip + h*mouse.x + v*mouse.y;
        
        
        GLuint mousePosition = mouseShaderProgram->uniform("mousePosition");

        glUniform3fv(mousePosition, 1, value_ptr(pos));
    
    }
    
    mouseShaderProgram->disable();
    
}

void TessMeshApp::Render(){
    App::Render();
    
    meshShaderProgram->use();
    mesh->Draw(GL_PATCHES);
    meshShaderProgram->disable();
    
    
    mouseShaderProgram->use();
    mouse->Draw(GL_POINTS);
    mouseShaderProgram->disable();

}