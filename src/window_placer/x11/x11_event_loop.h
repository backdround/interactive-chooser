#pragma once
#include <thread>
#include <functional>

#include <xcb/randr.h>
#include <xcb/xcb_ewmh.h>

class X11_event_loop {

public:
    using events_handler_t = std::function<void(const xcb_generic_event_t*)>;

    X11_event_loop(xcb_connection_t* xcb, xcb_screen_t* screen,
                   events_handler_t handlers);
    ~X11_event_loop();

private:
    void run();
    bool closure_event(const xcb_generic_event_t* event);

    xcb_connection_t* const xcb_;
    xcb_screen_t* const screen_;
    events_handler_t events_handler_;

    std::thread loop_thread_;
};
