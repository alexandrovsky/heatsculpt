//
//  TransformFeedbackMesh.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 23.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "TransformFeedbackMesh.h"

TransformFeedbackMesh::TransformFeedbackMesh():Mesh(){
    
    glGenVertexArrays(1, &dest_vao);
    glBindVertexArray(dest_vao);
    glBindVertexArray(0);
    
}
TransformFeedbackMesh::~TransformFeedbackMesh(){
    glDeleteVertexArrays(1, &dest_vao);
    for (int i = 0; i < dest_attributes.size(); i++) {
        Attribute attrib = attributes[i];
        glDeleteBuffers(1, &attrib.id);
        
    }
}

template<typename T> GLuint addTransformFeedbackVBO(vector<T> vector, Attribute& attribute){

    Attribute dest_attrib;
    addVBO(vector, attribute);
    std::memcpy(&dest_attrib, &attribute, sizeof (Attribute));
    addVBO(vector, dest_attrib);
    return 1;
}