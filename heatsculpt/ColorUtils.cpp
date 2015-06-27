//
//  ColorUtils.cpp
//  heatsculpt
//
//  Created by Dmitry Alexandrovsky on 20.06.15.
//  Copyright (c) 2015 Dmitry Alexandrovsky. All rights reserved.
//

#include "ColorUtils.h"



void hsv2rgb(unsigned int hue, unsigned int sat, unsigned int val, \
             unsigned char * r, unsigned char * g, unsigned char * b, unsigned char maxBrightness ) {
    unsigned int H_accent = hue/60;
    unsigned int bottom = ((255 - sat) * val)>>8;
    unsigned int top = val;
    unsigned char rising  = ((top-bottom)  *(hue%60   )  )  /  60  +  bottom;
    unsigned char falling = ((top-bottom)  *(60-hue%60)  )  /  60  +  bottom;
    
    switch(H_accent) {
        case 0:
            *r = top;
            *g = rising;
            *b = bottom;
            break;
            
        case 1:
            *r = falling;
            *g = top;
            *b = bottom;
            break;
            
        case 2:
            *r = bottom;
            *g = top;
            *b = rising;
            break;
            
        case 3:
            *r = bottom;
            *g = falling;
            *b = top;
            break;
            
        case 4:
            *r = rising;
            *g = bottom;
            *b = top;
            break;
            
        case 5:
            *r = top;
            *g = bottom;
            *b = falling;
            break;
    }
    // Scale values to maxBrightness
    *r = *r * maxBrightness/255;
    *g = *g * maxBrightness/255;
    *b = *b * maxBrightness/255;
}


void generateColors(unsigned int num_of_colors, vector<vec3>& colors){
    
    unsigned int h = 0, s = 255, v = 255;
    unsigned char r_255 = 0, g_255 = 0, b_255 = 0;
    unsigned char maxBrightnes = 255;
    
    
    float r, g, b;
    for (int i = 0; i < num_of_colors; i++) {
        
        if (i % 3 == 0) {
            hsv2rgb(h, s, v, &r_255, &g_255, &b_255, maxBrightnes);
            r = (float)r_255/255.0f;
            g = (float)g_255/255.0f;
            b = (float)b_255/255.0f;
            
            h += 360 / (num_of_colors/3);
        }
        
        colors.push_back(glm::vec3(r, g, b));


    }
}