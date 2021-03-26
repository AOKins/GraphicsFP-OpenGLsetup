#include "../headers/app.h"


// Handler for general events
// Input: event - pointer to SDL_Event type
//        deltaTime - time since last frame render, useful if input relies on relative to last frame
// Output: identifies a relevant event and calls appriopriate metehod
void application::event(SDL_Event * event, double deltaTime) {
    // Determine the event and call appriopriate method
    switch (event->type) {
        case (SDL_QUIT):
            close();
            break;
        /* Disabling mouse input
        case (SDL_MOUSEMOTION):
            onMouseMove(&event->motion);
            break;
        case (SDL_MOUSEBUTTONDOWN):
            onMouseButton(&event->button);
            break;
        */
        case (SDL_KEYDOWN):
            onKeyPress(&event->key, deltaTime);
            break;
    }
}

// Handle for mouse button presses
// Input: button - mouse button event object
// Output: Left mouse button increases mouseClick counter, right mouse button decrements the mouseClick counter
void application::onMouseButton(SDL_MouseButtonEvent * button) {
    // Identify the mouse button event type
    switch (button->type) {
        case SDL_MOUSEBUTTONDOWN:
            switch (button->button) {  // Identify which button is being pressed
                case SDL_BUTTON_LEFT:
                    ++mouseClick;
                    break;
                case SDL_BUTTON_RIGHT:
                    --mouseClick;
                    break;
            }
            break;
    }
}

// Handler for mouse movement
// Input: mouse - mouse motion event
// Output: Depending on mouseClick count, one (or none) of the vertices will have their x,y positions updated to where the mouse is
void application::onMouseMove(SDL_MouseMotionEvent * mouse) {
    // Get the x & y coordinates of the mouse
    // int x = mouse->x;
    // int y = mouse->y;


}

// Handler for keyboard presses - https://wiki.libsdl.org/SDL_Scancode was useful in identifying specific keys wanted
// Input: key_event - SDL keyboard event object
// Output: Depending on recognized key press, changes the mode for transformation behavior the application will do in render()
void application::onKeyPress(SDL_KeyboardEvent * key_event, double deltaTime) {
    switch(key_event->keysym.scancode) {
        case (SDL_SCANCODE_ESCAPE):
            this->close();
            break;
        // Function keys change polygon mode between filled and lines (wireframe)
        case (SDL_SCANCODE_F1):
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case (SDL_SCANCODE_F2):
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(3.0f);
            break;
        // Adjust FOV
        case (SDL_SCANCODE_F5):
            mainCamera.setFOV(mainCamera.getFOV()-deltaTime);
            break;
        case (SDL_SCANCODE_F6):
            mainCamera.setFOV(mainCamera.getFOV()+deltaTime);
            break;
        // Movement inputs
        case (SDL_SCANCODE_W):
            mainCamera.updatePos(-deltaTime, Z);
            break;
        case (SDL_SCANCODE_S):
            mainCamera.updatePos(deltaTime, Z);
            break;
        case (SDL_SCANCODE_A):
            mainCamera.updatePos(-deltaTime, X);
            break;
        case (SDL_SCANCODE_D):
            mainCamera.updatePos(deltaTime, X);
            break;
        case (SDL_SCANCODE_Z):
            mainCamera.updatePos(deltaTime, Y);
            break;
        case (SDL_SCANCODE_X):
            mainCamera.updatePos(-deltaTime, Y);
            break;/*
        case (SDL_SCANCODE_UP):
            mainCamera.setFront(glm::vec3( 0.0f, 1.0f, 0.0f));
            break;
        case (SDL_SCANCODE_DOWN):
            mainCamera.setFront(glm::vec3( 0.0f,-1.0f, 0.0f));
            break;
        case (SDL_SCANCODE_LEFT):
            mainCamera.setFront(glm::vec3(-1.0f, 0.0f, 0.0f));
            break;
        case (SDL_SCANCODE_RIGHT):
            mainCamera.setFront(glm::vec3( 1.0f, 0.0f, 0.0f));
            break;
        */
        default: // If unrecognized, default to doing nothing
            break;
    }
}
