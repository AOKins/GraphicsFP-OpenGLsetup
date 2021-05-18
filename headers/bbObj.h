#ifndef _BBOBJ_H_
#define _BBOBJ_H_

#include "object.h"
#include "bb.h"

// Struct for connecting bound box with objects
struct bbObj {
    aabb3 boundBox;
    object * obj;

    bbObj(object * set_obj);
};

#endif
