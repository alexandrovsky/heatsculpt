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
//#include "TessMeshApp.h"
//#include "TransformFeedbackApp.h"
//#include "TransformFeedbackMeshApp.h"


#include "HeatSculptApp.h"

int main(int argc, char* argv[]) {
    
//    TestApp tApp("Geometry Shader Test", 800, 600);
//    return tApp.Start();
    
//    MeshTestApp mtApp("Mesh Test", 800, 600);
//    return mtApp.Start();
    
//    TessMeshApp tessApp("tess", true);
//    return tessApp.Start();
    
//    TransformFeedbackApp tfApp("transform feedback", true);
//    return tfApp.Start();
    
//    TransformFeedbackMeshApp tfmApp("transform feedback mesh", false);
//    return tfmApp.Start();
    
    HeatSculptApp app("heatsculpt", false);
    return app.Start();
    
}








