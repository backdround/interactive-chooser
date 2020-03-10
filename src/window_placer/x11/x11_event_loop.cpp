#include <cstring>

#include "x11_event_loop.h"

X11_event_loop::X11_event_loop(xcb_connection_t* xcb, xcb_screen_t* screen,
                               events_handler_t handlers)
    : xcb_(xcb), screen_(screen), events_handler_(handlers) {

    // Setup event filter.
    const uint32_t event_filter[] = {XCB_EVENT_MASK_PROPERTY_CHANGE};
    xcb_change_window_attributes(xcb_, screen_->root, XCB_CW_EVENT_MASK,
                                 event_filter);
    xcb_flush(xcb_);

    loop_thread_ = std::thread(&X11_event_loop::run, this);
}


X11_event_loop::~X11_event_loop() {
    // Sending closure event.

    // Window creation.
    const xcb_window_t window = xcb_generate_id(xcb_);
    xcb_create_window(xcb_, XCB_COPY_FROM_PARENT, window, screen_->root, 0, 0,
                      1, 1, 0, XCB_WINDOW_CLASS_INPUT_ONLY,
                      screen_->root_visual, 0, nullptr);

    // Client message creation.
    xcb_client_message_event_t event;
    memset(&event, 0, sizeof(event));
    event.response_type = XCB_CLIENT_MESSAGE;
    event.format = 32;
    event.window = window;

    // Sending event.
    xcb_send_event(xcb_, false, window, XCB_EVENT_MASK_NO_EVENT,
                   reinterpret_cast<const char*>(&event));
    xcb_destroy_window(xcb_, window);
    xcb_flush(xcb_);


    // Waiting loop closure event handling.
    loop_thread_.join();
}


void X11_event_loop::run() {
    while (auto event = xcb_wait_for_event(xcb_)) {
        if (!event) {
            continue;
        }

        if (closure_event(event)) {
            free(event);
            break;
        }

        events_handler_(event);
        free(event);
    }
}


bool X11_event_loop::closure_event(const xcb_generic_event_t* event) {
    if ((event->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
        return true;
    }
    return false;
}
