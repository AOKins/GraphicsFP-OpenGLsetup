#ifndef _VERTEXTURE_H_
#define _VERTEXTURE_H_

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "vertex.h"
#include "color.h"

// Basic struct that holds vertex coordinates, color info for the vertex, and 2D texture coordinates
struct vertexture {
    // Coordinate of the vertex
    vertex coords;
    // Color of the vertex
    color col;

    // The X,Y coordinates for textures
    float tex_x;
    float tex_y;

    vertexture(vertex set_coords, color set_color, float set_x, float set_y) {
        this->coords = set_coords;
        this->col = set_color;
        this->tex_x = set_x;
        this->tex_y = set_y;
    }
};

#endif
