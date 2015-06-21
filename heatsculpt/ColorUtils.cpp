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