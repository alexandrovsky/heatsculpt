//
//  ColorUtils.h
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 20.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#ifndef __heatsculpt__ColorUtils__
#define __heatsculpt__ColorUtils__

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
using namespace std;
using namespace glm;
void hsv2rgb(unsigned int hue, unsigned int sat, unsigned int val, \
             unsigned char * r, unsigned char * g, unsigned char * b, unsigned char maxBrightness );

void generateSingleColor(unsigned int num_of_colors, vector<vec3>& colors, vec3 color);
void generateColors(unsigned int num_of_colors, vector<vec3>& colors);


#endif /* defined(__heatsculpt__ColorUtils__) */
