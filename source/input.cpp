#include "../headers/app.h"

// Handler for general events
// Input: event - pointer to SDL_Event type
// Output: identifies a relevant event and calls appriopriate metehod
void application::event(SDL_Event * event) {
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
            onKeyPress(&event->key);
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
    int x = mouse->x;
    int y = mouse->y;

    // Ignores mouse cords every 7th click
    if (mouseClick % 7 != 6) {
        // Adjusting the x,y coordinates of the mouse into a value range between -1 and +1
        GLfloat x_cords =  2 * GLfloat(x)/float(window_width)  - 1.0f;
        GLfloat y_cords = -2 * GLfloat(y)/float(window_height) + 1.0f;
        // Setting the triangle vertex cordinates (one of the three dependent on mouseClick count) to mouse coordinates
        triangleData[(abs(mouseClick) % 3)].vertex.x = x_cords;
        triangleData[(abs(mouseClick) % 3)].vertex.y = y_cords;
    }
}

// Handler for keyboard presses
// Input: key_event - SDL keyboard event object
// Output: Depending on recognized key press, changes the mode for transformation behavior the application will do in render()
void application::onKeyPress(SDL_KeyboardEvent * key_event) {
    switch(key_event->keysym.scancode) {
        case (SDL_SCANCODE_F1):
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case (SDL_SCANCODE_F2):
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(3.0f);
            break;
        case (SDL_SCANCODE_SPACE): // Space key force NONE mode (no transformations)
            transformMode = NONE;
            break;
        // 1-7 keys perform specific tranform modes
        case (SDL_SCANCODE_T):
            transformMode = TRANSLATION;
            break;
        case (SDL_SCANCODE_2):
            transformMode = SCALE;
            break;
        case (SDL_SCANCODE_X):
            transformMode = ROTATION_X;
            break;
        case (SDL_SCANCODE_Y):
            transformMode = ROTATION_Y;
            break;
        case (SDL_SCANCODE_Z):
            transformMode = ROTATION_Z;
            break;
        case (SDL_SCANCODE_R):
            transformMode = REFLECTION;
            break;
        case (SDL_SCANCODE_5):
            transformMode = SHEARING;
            break;
        case (SDL_SCANCODE_I):
            //shaderApp->setBool("cancel", !shaderApp->cancel);;
            //shaderApp->cancel = !shaderApp->cancel;
            break;
        case (SDL_SCANCODE_P):
            transformMode = PROJECTION;
            break;
        case (SDL_SCANCODE_F): // F key engages FUN mode
            transformMode = FUN;
            break;
        default: // If unrecognized, default to NONE
            transformMode = NONE;
            break;
    }
}
