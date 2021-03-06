#pragma once
#include <QObject>

#include <xcb/randr.h>
#include <xcb/xcb_ewmh.h>

#include <vector>

#include "x11_event_loop.h"

#include "monitor_geometry.h"

class X11_notifier : public Monitor_geometry {
    Q_OBJECT

public:
    X11_notifier();
    ~X11_notifier();

    QRect current_monitor_geometry() override;

// base signals:
    // void current_monitor_changed(const QRect& geometry);

private slots:
    void event_handler(const xcb_generic_event_t* event);

private:
    // Monitors
    void get_monitors_info();
    void update_desktop();

    std::vector<QRect> monitors;
    QRect current_monitor;

    // Xcb
    xcb_connection_t* xcb;
    xcb_screen_t* screen;
    xcb_window_t hidden_window;
    xcb_ewmh_connection_t* ewmh;

    // X11 loop
    X11_event_loop* loop_ = nullptr;
};
