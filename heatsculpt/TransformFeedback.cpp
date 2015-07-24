//
//  TransformFeedback.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 24.07.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "TransformFeedback.h"

TransformFeedback::TransformFeedback(){
}

TransformFeedback::~TransformFeedback(){
    delete shaderProgram;
}


bool Init(){
    return true;
}