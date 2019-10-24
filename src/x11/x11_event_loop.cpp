#include "x11_event_loop.h"

X11_event_loop::X11_event_loop(xcb_connection_t* xcb, xcb_screen_t* screen)
    : xcb_(xcb), screen_(screen) {
    // Setup event filter.
    const uint32_t event_filter[] = {XCB_EVENT_MASK_PROPERTY_CHANGE};
    xcb_change_window_attributes(xcb_, screen_->root, XCB_CW_EVENT_MASK,
                                 event_filter);
    xcb_flush(xcb_);
}

void X11_event_loop::run() {
    while (auto event = xcb_wait_for_event(xcb_)) {
        if (!event) {
            continue;
        }

        if (exit_loop_check(event)) {
            free(event);
            break;
        }

        if (active_desktop_changed_check(event)) {
            emit active_desktop_changed();
        }

        free(event);
    }
}

bool X11_event_loop::active_desktop_changed_check(
    const xcb_generic_event_t* event) {
    if (event->response_type == XCB_PROPERTY_NOTIFY) {
        // Get atom name.
        auto property_event =
            reinterpret_cast<const xcb_property_notify_event_t*>(event);
        auto name_cookie = xcb_get_atom_name(xcb_, property_event->atom);
        auto name_reply = xcb_get_atom_name_reply(xcb_, name_cookie, nullptr);
        auto atom_name = xcb_get_atom_name_name(name_reply);

        // Compare with desktop changed atom name.
        auto desktop_changed_atom_name = "_NET_CURRENT_DESKTOP";
        if (memcmp(atom_name, desktop_changed_atom_name,
                   name_reply->name_len) == 0) {
            return true;
        }
    }
    return false;
}

bool X11_event_loop::exit_loop_check(const xcb_generic_event_t* event) {
    if ((event->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
        return true;
    }
    return false;
}

void X11_event_loop::stop() {
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
}
