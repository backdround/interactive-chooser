#pragma once
#include <QObject>

#include <xcb/randr.h>
#include <xcb/xcb_ewmh.h>

class X11_event_loop : public QObject {
    Q_OBJECT

public:
    X11_event_loop(xcb_connection_t* xcb, xcb_screen_t* screen);
    void stop();

public slots:
    void run();

signals:
    void active_desktop_changed();

private:
    bool exit_loop_check(const xcb_generic_event_t* event);
    bool active_desktop_changed_check(const xcb_generic_event_t* event);

    xcb_connection_t* const xcb_;
    xcb_screen_t* const screen_;
};
