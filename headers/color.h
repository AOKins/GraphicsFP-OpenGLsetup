#ifndef _COLOR_H_
#define _COLOR_H_

struct color {
    float r,g,b,o;
    
    color() {
        this->r = 1.0f;
        this->g = 0.0f;
        this->b = 1.0f;
        this->o = 1.0f;
    }

    // Constructor to set the x,y,z values
    color(float set_r, float set_g, float set_b, float set_o) {
        this->r = set_r;
        this->g = set_g;
        this->b = set_b;
        this->o = set_o;
    }
};

#endif