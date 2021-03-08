#include "../headers/app.h"

void application::event(SDL_Event * event) {
    // Determine the event and call appriopriate method
    switch (event->type) {
        case (SDL_QUIT):
            close();
            break;
        case (SDL_MOUSEMOTION):
            onMouseMove(&event->motion);
            break;
        case (SDL_MOUSEBUTTONDOWN):
            onMouseButton(&event->button);
            break;
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
        GLfloat x_cords = 2 * GLfloat(x)/float(window_width) - 1.0f;
        GLfloat y_cords = -2 * GLfloat(y)/float(window_height) + 1.0f;
        // Setting the triangle vertex cordinates (one of the three dependent on mouseClick count) to mouse coordinates
        triangleVertices[0 + 3*(abs(mouseClick) % 3)] = x_cords;
        triangleVertices[1 + 3*(abs(mouseClick) % 3)] = y_cords;
    }
}
