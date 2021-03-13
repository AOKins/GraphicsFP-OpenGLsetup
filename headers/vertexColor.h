#include <glm/glm.hpp>

// Simple struct to help manage the vertices and their colors
struct vertexColor {
    glm::vec4 vertex;
    glm::vec4 color;

    vertexColor();
    vertexColor(glm::vec4 set_vertex, glm::vec4 set_color);    
};

vertexColor::vertexColor() {
    vertex = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);    // Defualt position is origin
    color  = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);    // Default is a magenta color
}

vertexColor::vertexColor(glm::vec4 set_vertex, glm::vec4 set_color) {
    vertex = set_vertex;
    color = set_color;
}
