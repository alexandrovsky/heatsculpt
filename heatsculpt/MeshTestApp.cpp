//
//  MeshTestApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 17.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "MeshTestApp.h"
#include <map>

GLuint getMidPoint(GLuint p1, GLuint p2, map<GLuint64, GLuint>& indexCache, vector<vec3>& vertices){
    bool firstIsSmaller = p1 < p2;
    GLuint64 smallerIndex = firstIsSmaller ? p1 : p2;
    GLuint64 greaterIndex = firstIsSmaller ? p2 : p1;
    
    GLuint64 key = (smallerIndex << 32) + greaterIndex;
    
    if (indexCache[key]) {
        return indexCache[key];
    }
    
    vertices[p1] = glm::normalize(vertices[p1]);
    vec3 point1 = vertices[p1];
    
    vertices[p2] = glm::normalize(vertices[p2]);
    vec3 point2 = vertices[p2];
    
    vec3 middle = vec3((point1.x + point2.x) / 2.0f,
                       (point1.y + point2.y) / 2.0f,
                       (point1.z + point2.z) / 2.0f);
    
    // add vertex makes sure point is on unit sphere
    vertices.push_back(glm::normalize(middle));
    GLuint i = (GLuint)vertices.size()-1;
    
    // store it, return index
    indexCache[key] = i;
    return i;

}

void createTetraedron(vector<vec3>& vertices, vector<GLuint>& indices){
    
    vertices.push_back(glm::vec3(  1.0,  1.0,  1.0));
    vertices.push_back(glm::vec3( -1.0, -1.0,  1.0));
    vertices.push_back(glm::vec3( -1.0,  1.0, -1.0));
    vertices.push_back(glm::vec3(  1.0, -1.0, -1.0));
    
    
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);
    
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(3);
    
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);
    
}

void createIcosphere(vector<vec3>& vertices, vector<GLuint>& indices, GLuint subdivisions=3){
    GLfloat t = (1.0f + sqrtf(5.0)) /2.0f;
    
    // create 12 vertices of a icosahedron
    
    vertices.push_back(vec3(-1,  t,  0));
    vertices.push_back(vec3( 1,  t,  0));
    vertices.push_back(vec3(-1, -t,  0));
    vertices.push_back(vec3( 1, -t,  0));
    
    vertices.push_back(vec3( 0, -1,  t));
    vertices.push_back(vec3( 0,  1,  t));
    vertices.push_back(vec3( 0, -1, -t));
    vertices.push_back(vec3( 0,  1, -t));
    
    vertices.push_back(vec3( t,  0, -1));
    vertices.push_back(vec3( t,  0,  1));
    vertices.push_back(vec3(-t,  0, -1));
    vertices.push_back(vec3(-t,  0,  1));
    
    // create 20 triangles of the icosahedron


    
    
    
    
    // 5 faces around point 0
    indices.push_back(0); indices.push_back(11); indices.push_back(5);
    indices.push_back(0); indices.push_back(5); indices.push_back(1);
    indices.push_back(0); indices.push_back(1); indices.push_back(7);
    indices.push_back(0); indices.push_back(7); indices.push_back(10);
    indices.push_back(0); indices.push_back(10); indices.push_back(11);

    
    // 5 adjacent faces
    indices.push_back(1); indices.push_back(5); indices.push_back(9);
    indices.push_back(5); indices.push_back(11); indices.push_back(4);
    indices.push_back(11); indices.push_back(10); indices.push_back(2);
    indices.push_back(10); indices.push_back(7); indices.push_back(6);
    indices.push_back(7); indices.push_back(1); indices.push_back(8);

    
    // 5 faces around point 3
    indices.push_back(3); indices.push_back(9); indices.push_back(4);
    indices.push_back(3); indices.push_back(4); indices.push_back(2);
    indices.push_back(3); indices.push_back(2); indices.push_back(6);
    indices.push_back(3); indices.push_back(6); indices.push_back(8);
    indices.push_back(3); indices.push_back(8); indices.push_back(9);

    
//    // 5 adjacent faces
    indices.push_back(4); indices.push_back(9); indices.push_back(5);
    indices.push_back(2); indices.push_back(4); indices.push_back(11);
    indices.push_back(6); indices.push_back(2); indices.push_back(10);
    indices.push_back(8); indices.push_back(6); indices.push_back(7);
    indices.push_back(9); indices.push_back(8); indices.push_back(1);
    
    
    //-------------
    
    for (int div= 0; div < subdivisions; div++) {
        
        map<GLuint64, GLuint> indexCache;
        
        vector<GLuint>newIndices;
        
        for (int i = 0; i< indices.size(); i+=3) {
            
            GLuint i0 = indices[i+0];
            GLuint i1 = indices[i+1];
            GLuint i2 = indices[i+2];
            
            GLuint a = getMidPoint(i0, i1, indexCache, vertices);
            GLuint b = getMidPoint(i1, i2, indexCache, vertices);
            GLuint c = getMidPoint(i2, i0, indexCache, vertices);
            
            newIndices.push_back(i0); newIndices.push_back(a); newIndices.push_back(c);
            newIndices.push_back(i1); newIndices.push_back(b); newIndices.push_back(a);
            newIndices.push_back(i2); newIndices.push_back(c); newIndices.push_back(b);
            newIndices.push_back(a);  newIndices.push_back(b);  newIndices.push_back(c);
        }
        
        indices.clear();
        indices.reserve(newIndices.size());
//        copy(newIndices.begin(), newIndices.end(), indices.begin());
        for (int i = 0; i < newIndices.size(); i++) {
            indices.push_back(newIndices[i]);
            
        }
        
    }
    
    
    

}


MeshTestApp::MeshTestApp(const std::string& window_title, int window_width, int window_height):
App::App(window_title, window_width, window_height)
{
    
}



MeshTestApp::~MeshTestApp()
{
    delete mesh;
}


bool MeshTestApp::Init(){
    bool res = App::Init();
    if (!res) { return res; }
    
    vertexShader = new Shader(GL_VERTEX_SHADER);
    vertexShader->loadFromString(vertexShaderSrc);
    vertexShader->compile();
    
    fragmentShader = new Shader(GL_FRAGMENT_SHADER);
    fragmentShader->loadFromString(fragmentShaderSrc);
    fragmentShader->compile();
    
    
    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader(*vertexShader);
    shaderProgram->attachShader(*fragmentShader);
    shaderProgram->linkProgram();
    shaderProgram->use();
    
    vector<vec3> vertices;
    vector<GLuint> indices;
    
    //createTetraedron(vertices, indices);
    createIcosphere(vertices, indices);
    
    mesh = new Mesh(shaderProgram, vertices, indices);
    
    
    
    return res;
}

void MeshTestApp::Update(){
    App::Update();
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(0.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
    
    //model = glm::translate(model, vec3(0.5f, 0.0f, 0.0f));
    
    mesh->modelMatrix = model;
    mesh->viewMatrix = camera.view;
    mesh->projectionMatrix = camera.projection;
    mesh->Update();
    
}

void MeshTestApp::Render(){
    App::Render();
    
    mesh->Draw();
}