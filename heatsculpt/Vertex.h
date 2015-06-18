//
//  Vertex.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 15.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__Vertex__
#define __heatsculpt__Vertex__
#include <glm/glm.hpp>
struct Vertex {

    Vertex(const glm::vec3& pos){
        this->position = pos;
    }
    glm::vec3 position;
    
};

#endif /* defined(__heatsculpt__Vertex__) */
