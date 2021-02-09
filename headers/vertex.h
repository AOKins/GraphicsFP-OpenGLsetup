#ifndef _VERTEX_H_
#define _VERTEX_H_

// Simple structure to contain the 3D components of a vertex for GL
struct vertex {
    float x,y,z;
    // Default constructor, sets components to 0's
    vertex() {
        this->x=0;
        this->y=0;
        this->z=0;
    }
    // Constructor to set the x,y,z values
    vertex(float x_i, float y_i, float z_i) {
        this->x = x_i;
        this->y = y_i;
        this->z = z_i;
    }
};

#endif
