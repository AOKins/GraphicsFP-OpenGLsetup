#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include <glm/glm.hpp>
#include <GL/gl.h>

// Simple class for basic lighting points
class light {
protected:
    glm::vec3 position; // Point location of the light
    GLfloat color[3]; // RGB color values from light
public:
    light();
    light(glm::vec3 iPos, glm::vec3 iColor);

    // Simple setters
    void setPos(glm::vec3 set_pos);
    void setColor(glm::vec3 set_pos);
    // Simple getters
    void getPos(glm::vec3 set_pos);
    void getColor(glm::vec3 set_pos);
};




#endif