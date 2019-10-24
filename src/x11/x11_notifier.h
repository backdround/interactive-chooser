#pragma once
#include <QObject>
#include <QRect>
#include <QThread>

#include <xcb/randr.h>
#include <xcb/xcb_ewmh.h>

#include <vector>

class X11_event_loop;

class X11_notifier : public QObject {
    Q_OBJECT
public:
    static X11_notifier* get_instance();

    QRect get_current_monitor_geometry();

signals:
    void current_monitor_changed(const QRect& screen);

private slots:
    void update_current_monitor();

private:
    // Singleton
    X11_notifier();
    ~X11_notifier();
    inline static X11_notifier* instance = nullptr;

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
    X11_event_loop* loop;
    QThread loop_thread;
};
