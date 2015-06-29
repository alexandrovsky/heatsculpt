//
//  TessMeshTestApp.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 21.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "TessMeshTestApp.h"
#include "MeshUtils.h"
#include <glm/gtc/matrix_access.hpp>
//#include <GLEW/GLEW.h>
#include <gl/glew.h>
Shader* vertexShader;
Shader* tessControlShader;
Shader* tessEvalShader;
Shader* geometryShader;
Shader* fragmentShader;



typedef struct {
    GLuint Projection;
    GLuint Modelview;
    GLuint NormalMatrix;
    GLuint LightPosition;
    GLuint AmbientMaterial;
    GLuint DiffuseMaterial;
    GLuint TessLevelInner;
    GLuint TessLevelOuter;
} ShaderUniforms;


static float TessLevelInner;
static float TessLevelOuter;
static mat3 NormalMatrix;
static ShaderUniforms Uniforms;


ShaderProgram* shaderProgram;


const string vertexShaderSrc =
GLSL(
     
     in vec4 Position;
     out vec3 vPosition;
     
     void main()
    {
        vPosition = Position.xyz;
    }
);


const string tessControlShaderSrc =
GLSL(
     layout(vertices = 3) out;
     in vec3 vPosition[];
     out vec3 tcPosition[];
     uniform float TessLevelInner;
     uniform float TessLevelOuter;
     
//     #define ID gl_InvocationID
     
     void main()
     {
         tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
         if (gl_InvocationID == 0) {
             gl_TessLevelInner[0] = TessLevelInner;
             gl_TessLevelOuter[0] = TessLevelOuter;
             gl_TessLevelOuter[1] = TessLevelOuter;
             gl_TessLevelOuter[2] = TessLevelOuter;
         }
     }
);


const string tessEvalShaderSrc =
GLSL(
     layout(triangles, equal_spacing, cw) in;
     in vec3 tcPosition[];
     out vec3 tePosition;
     out vec3 tePatchDistance;
     uniform mat4 Projection;
     uniform mat4 Modelview;
     
     void main()
     {
         vec3 p0 = gl_TessCoord.x * tcPosition[0];
         vec3 p1 = gl_TessCoord.y * tcPosition[1];
         vec3 p2 = gl_TessCoord.z * tcPosition[2];
         tePatchDistance = gl_TessCoord;
         tePosition = normalize(p0 + p1 + p2);
         gl_Position = Projection * Modelview * vec4(tePosition, 1);
     }
);



const string geometryShaderSrc =
GLSL(
     uniform mat4 Modelview;
     uniform mat3 NormalMatrix;
     layout(triangles) in;
     layout(triangle_strip, max_vertices = 3) out;
     in vec3 tePosition[3];
     in vec3 tePatchDistance[3];
     out vec3 gFacetNormal;
     out vec3 gPatchDistance;
     out vec3 gTriDistance;
     
     void main()
    {
        vec3 A = tePosition[2] - tePosition[0];
        vec3 B = tePosition[1] - tePosition[0];
        gFacetNormal = NormalMatrix * normalize(cross(A, B));
    
        gPatchDistance = tePatchDistance[0];
        gTriDistance = vec3(1, 0, 0);
        gl_Position = gl_in[0].gl_Position; EmitVertex();
    
        gPatchDistance = tePatchDistance[1];
        gTriDistance = vec3(0, 1, 0);
        gl_Position = gl_in[1].gl_Position; EmitVertex();
    
        gPatchDistance = tePatchDistance[2];
        gTriDistance = vec3(0, 0, 1);
        gl_Position = gl_in[2].gl_Position; EmitVertex();
    
        EndPrimitive();
    }
);

// fragment:
const char* fragmentShaderSrc =
GLSL(
     out vec4 FragColor;
     in vec3 gFacetNormal;
     in vec3 gTriDistance;
     in vec3 gPatchDistance;
     in float gPrimitive;
     uniform vec3 LightPosition;
     uniform vec3 DiffuseMaterial;
     uniform vec3 AmbientMaterial;
     
     float amplify(float d, float scale, float offset)
     {
         d = scale * d + offset;
         d = clamp(d, 0, 1);
         d = 1 - exp2(-2*d*d);
         return d;
     }
     
     void main()
     {
         vec3 N = normalize(gFacetNormal);
         vec3 L = LightPosition;
         float df = abs(dot(N, L));
         vec3 color = AmbientMaterial + df * DiffuseMaterial;
    
         float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
         float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
         color = amplify(d1, 40, -0.5) * amplify(d2, 60, -0.5) * color;
    
         FragColor = vec4(color, 1.0);
     }
);









TessMeshTestApp::TessMeshTestApp(const std::string& window_title, int window_width, int window_height):
App::App(window_title, window_width, window_height)
{
    
}



TessMeshTestApp::~TessMeshTestApp()
{
    delete mesh;
}


bool TessMeshTestApp::Init(){
    bool res = App::Init();
    if (!res) { return res; }
    
    vertexShader = new Shader(GL_VERTEX_SHADER);
    vertexShader->loadFromString(vertexShaderSrc);
    vertexShader->compile();
    
    
    tessControlShader = new Shader(GL_TESS_CONTROL_SHADER);
    tessControlShader->loadFromString(tessControlShaderSrc);
    tessControlShader->compile();
    
    
    tessEvalShader = new Shader(GL_TESS_EVALUATION_SHADER);
    tessEvalShader->loadFromString(tessEvalShaderSrc);
    tessEvalShader->compile();
    
    geometryShader = new Shader(GL_GEOMETRY_SHADER_ARB);
    geometryShader->loadFromString(geometryShaderSrc);
    geometryShader->compile();
    
    
    
    fragmentShader = new Shader(GL_FRAGMENT_SHADER);
    fragmentShader->loadFromString(fragmentShaderSrc);
    fragmentShader->compile();
    
    
    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader(*vertexShader);
    shaderProgram->attachShader(*tessControlShader);
    shaderProgram->attachShader(*tessEvalShader);
    shaderProgram->attachShader(*geometryShader);
    shaderProgram->attachShader(*fragmentShader);
    shaderProgram->linkProgram();
    shaderProgram->use();
    
    
    
    
    TessLevelInner = 3;
    TessLevelOuter = 2;
    
    
    
    Uniforms.Projection = shaderProgram->addUniform("Projection");
    Uniforms.Modelview = shaderProgram->addUniform("Modelview");
    Uniforms.NormalMatrix = shaderProgram->addUniform("NormalMatrix");
    Uniforms.LightPosition = shaderProgram->addUniform("LightPosition");
    Uniforms.AmbientMaterial = shaderProgram->addUniform("AmbientMaterial");
    Uniforms.DiffuseMaterial = shaderProgram->addUniform("DiffuseMaterial");
    Uniforms.TessLevelInner = shaderProgram->addUniform("TessLevelInner");
    Uniforms.TessLevelOuter = shaderProgram->addUniform("TessLevelOuter");
    
    
    
    const GLuint Faces[] = {
        2, 1, 0,
        3, 2, 0,
        4, 3, 0,
        5, 4, 0,
        1, 5, 0,
        
        11, 6,  7,
        11, 7,  8,
        11, 8,  9,
        11, 9,  10,
        11, 10, 6,
        
        1, 2, 6,
        2, 3, 7,
        3, 4, 8,
        4, 5, 9,
        5, 1, 10,
        
        2,  7, 6,
        3,  8, 7,
        4,  9, 8,
        5, 10, 9,
        1, 6, 10 };
    
    vector<vec3> vertices;
    vertices.push_back(vec3(0.000f,  0.000f,  1.000f));
    vertices.push_back(vec3(0.894f,  0.000f,  0.447f));
    vertices.push_back(vec3(0.276f,  0.851f,  0.447f));
    vertices.push_back(vec3(-0.724f,  0.526f,  0.447f));
    vertices.push_back(vec3(-0.724f, -0.526f,  0.447f));
    vertices.push_back(vec3(0.276f, -0.851f,  0.447f));
    vertices.push_back(vec3(0.724f,  0.526f, -0.447f));
    vertices.push_back(vec3(-0.276f,  0.851f, -0.447f));
    vertices.push_back(vec3(-0.894f,  0.000f, -0.447f));
    vertices.push_back(vec3(-0.276f, -0.851f, -0.447f));
    vertices.push_back(vec3(0.724f, -0.526f, -0.447f));
    vertices.push_back(vec3(0.000f,  0.000f, -1.000f));
    


    
    

    
    vector<GLuint> indices;
    unsigned int sizeI = sizeof(Faces)/sizeof(Faces[0]);
    copy(&Faces[0], &Faces[sizeI], back_inserter(indices));
    
    
    //createTetraedron(vertices, indices);
//    createIcosahedron(vertices, indices);
    
    vector<Attribute>attributes;
    
    Attribute positionAttrib;
    positionAttrib.name = "Position";
    positionAttrib.num_of_components = 3;
    positionAttrib.data_type = GL_FLOAT;
    positionAttrib.buffer_type = GL_ARRAY_BUFFER;
    attributes.push_back(positionAttrib);

    
    

    
    
    
    
    
    mesh = new Mesh(shaderProgram,attributes, vertices, indices);
    
    
    
    return res;
}

void TessMeshTestApp::Update(){
    App::Update();
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(0.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
    
    //model = glm::translate(model, vec3(0.5f, 0.0f, 0.0f));
    
    mesh->modelMatrix = model;
    
    NormalMatrix = mat3();
    
    mesh->Update();
    
}

void TessMeshTestApp::Render(){
    App::Render();
    
    mesh->shaderProgram->use();
    
    glUniform1f(Uniforms.TessLevelInner, TessLevelInner);
    glUniform1f(Uniforms.TessLevelOuter, TessLevelOuter);
    
    vec4 lightPosition(0.25, 0.25, 1, 0);
    glUniform3fv(Uniforms.LightPosition, 1, &lightPosition.x);
    
    
    glUniformMatrix4fv(Uniforms.Projection, 1, 0, glm::value_ptr(camera.view));

    
    glUniformMatrix4fv(Uniforms.Projection, 1, 0, glm::value_ptr(camera.projection));
    glUniformMatrix4fv(Uniforms.Modelview, 1, 0, glm::value_ptr(mesh->modelMatrix));
    
    mat3 nm = transpose(NormalMatrix);
    
    float packed[9] = { column(nm, 0).x, column(nm, 1).x, column(nm, 2).x,
                        column(nm, 0).y, column(nm, 1).y, column(nm, 2).y,
                        column(nm, 0).z, column(nm, 0).z, column(nm, 2).z };
    glUniformMatrix3fv(Uniforms.NormalMatrix, 1, 0, &packed[0]);
    
    // Render the scene:
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glUniform3f(Uniforms.AmbientMaterial, 0.04f, 0.04f, 0.04f);
    glUniform3f(Uniforms.DiffuseMaterial, 0, 0.75, 0.75);
    glDrawElements(GL_PATCHES, mesh->getDrawCount(), GL_UNSIGNED_INT, 0);
    
    mesh->shaderProgram->disable();
    
    //mesh->Draw();
}

