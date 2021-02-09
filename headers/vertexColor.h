#ifndef _VERTEXCOLOR_H_
#define _VERTEXCOLOR_H_

#include "vertex.h"
#include "color.h"

// A simple struct to hold both vertex position data and color together
struct vertexColor {
    vertex loc;
    color col;

    vertexColor(vertex set_loc, color set_col) {
        this->loc = set_loc;
        this->col = set_col;
    }
};

#endif