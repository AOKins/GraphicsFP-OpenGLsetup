
#include "bbObj.h"
#include <algorithm>


bbObj::bbObj(object * set_obj) {
    this->obj = set_obj;
    float max_x, max_y, max_z, min_x, min_y, min_z;
    max_x = 0.0f;
    max_y = 0.0f;
    max_z = 0.0f;
    min_x = 0.0f;
    min_y = 0.0f;
    min_z = 0.0f;
    for (int i = 0; i < this->obj->verticies.size(); i++) {
        std::max(max_x, this->obj->verticies[i].x);
        std::max(max_y, this->obj->verticies[i].y);
        std::max(max_z, this->obj->verticies[i].z);
        std::min(min_x, this->obj->verticies[i].x);
        std::min(min_y, this->obj->verticies[i].y);
        std::min(min_z, this->obj->verticies[i].z);
    }
}

