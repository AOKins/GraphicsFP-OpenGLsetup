#include "../headers/app.h"

void application::event(SDL_Event * event) {
    // Determine the event and call appriopriate method
    switch (event->type) {
        case (SDL_QUIT):
            close();
            break;
        /*case (SDL_MOUSEMOTION):
            onMouseMove(&event->motion);
            break;
        case (SDL_MOUSEBUTTONDOWN):
            onMouseButton(&event->button);
            break;*/
        case (SDL_KEYDOWN):
            onKeyPress(&event->key);
    }
}

void application::onMouseButton(SDL_MouseButtonEvent * button) {
    // Identify the mouse button event type
    switch (button->type) {
        case SDL_MOUSEBUTTONDOWN:
            // Identify which button is being pressed
            switch (button->button) {
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

void application::onMouseMove(SDL_MouseMotionEvent * mouse_event) {
    // Get the x & y coordinates of the mouse
    int x = mouse_event->x;
    int y = mouse_event->y;

    // Ignores mouse cords every 4th click
    if (mouseClick % 4 != 3) {
        // Adjusting the x,y coordinates of the mouse into a value range between -1 and +1
        GLfloat x_cords =  2 * GLfloat(x)/float(window_width)  - 1.0f;
        GLfloat y_cords = -2 * GLfloat(y)/float(window_height) + 1.0f;
        // Setting the triangle vertex cordinates (one of the three dependent on mouseClick count) to mouse coordinates
        triangleVertices[(abs(mouseClick) % 3)].x = x_cords;
        triangleVertices[(abs(mouseClick) % 3)].y = y_cords;
    }
}

void application::onKeyPress(SDL_KeyboardEvent * key_event) {
    switch(key_event->keysym.scancode) {
        case (SDL_SCANCODE_0):
            transformMode = NONE;
            break;
        case (SDL_SCANCODE_1):
            transformMode = TRANSLATION;
            break;
        case (SDL_SCANCODE_2):
            transformMode = SCALE;
            break;
        case (SDL_SCANCODE_3):
            transformMode = ROTATION;
            break;
        case (SDL_SCANCODE_4):
            transformMode = REFLECTION;
            break;
        case (SDL_SCANCODE_5):
            transformMode = SHEARING;
            break;
        case (SDL_SCANCODE_6):
            transformMode = INVERSION;
            break;
        case (SDL_SCANCODE_7):
            transformMode = PROJECTION;
            break;
        case (SDL_SCANCODE_8):
            transformMode = FUN;
            break;
        default:
            transformMode = NONE;
            break;
    }
}