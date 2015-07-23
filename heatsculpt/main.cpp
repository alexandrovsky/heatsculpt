//
//  main.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 12.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include <iostream>

//#include "TestApp.h"
//#include "MeshTestApp.h"
#include "TessMeshApp.h"
//#include "TransformFeedbackApp.h"
#include "TransformFeedbackTessApp.h"


int main(int argc, char* argv[]) {
    
//    TestApp tApp("Geometry Shader Test");
//    return tApp.Start();
    
//    MeshTestApp mtApp("Mesh Test");
//    return mtApp.Start();
    
    TessMeshApp tessApp("tess", true);
    return tessApp.Start();
    
//    TransformFeedbackApp tfApp("transform feedback", true);
//    return tfApp.Start();
    
//    TransformFeedbackTessApp tfTessApp("transform feedback tesselation", true);
//    return tfTessApp.Start();
    
}




