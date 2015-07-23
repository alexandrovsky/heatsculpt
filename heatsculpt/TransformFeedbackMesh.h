//
//  TransformFeedbackMesh.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 23.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__TransformFeedbackMesh__
#define __heatsculpt__TransformFeedbackMesh__

#include <stdio.h>
#include "Mesh.h"

class TransformFeedbackMesh : public Mesh{
public:
    TransformFeedbackMesh();
    ~TransformFeedbackMesh();

    vector<Attribute> dest_attributes;
    
    inline GLuint getDestVAO(){
        return dest_vao;
    }
    
//    template<typename T> std::pair<GLuint, GLuint> addTransformFeedbackVBO(vector<T> vector, Attribute& attribute);
protected:

    GLuint dest_vao;
private:
    
    
    
};
#endif /* defined(__heatsculpt__TransformFeedbackMesh__) */
