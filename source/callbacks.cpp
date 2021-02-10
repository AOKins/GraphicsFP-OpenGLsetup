#ifndef _CALLBACKS_CPP_
#define _CALLBACKS_CPP_

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

// File for callback methods


// Simple initial error callback for GLFW provided by https://www.glfw.org/docs/latest/quick.html
void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

// Callback method for managing key input
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Checking when key is pressed
    if (GLFW_PRESS) {
        // Checking for keys
        switch (key) {
            case (GLFW_KEY_ESCAPE):
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            case (GLFW_KEY_1):
                std::cout << "Wireframe mode on\n";
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case (GLFW_KEY_2):
                std::cout << "Filled mode on\n";
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
        }
    }

}

// Currently, the simple resize via cursor option is disabled
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

#endif
