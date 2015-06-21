//
//  MeshUtils.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 21.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__MeshUtils__
#define __heatsculpt__MeshUtils__



#include <stdio.h>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <OpenGL/OpenGL.h>
using namespace std;
using namespace glm;


void createTetraedron(vector<vec3>& vertices, vector<GLuint>& indices);
void createIcosahedron(vector<vec3>& vertices, vector<GLuint>& indices);
void createIcosphere(vector<vec3>& vertices, vector<GLuint>& indices, GLuint subdivisions=3);

#endif /* defined(__heatsculpt__MeshUtils__) */
