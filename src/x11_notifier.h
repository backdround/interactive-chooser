#pragma once
#include <QObject>
#include <QRect>
#include <QThread>

#include <xcb/randr.h>
#include <xcb/xcb_ewmh.h>

#include <vector>

struct monitor {
    int x;
    int y;
    int width;
    int height;
};

class X11_loop : public QObject {
    Q_OBJECT
public:
 X11_loop(xcb_connection_t* xcb, xcb_window_t root)
     : xcb_(xcb), root_(root) {}

 void run();

signals:
    void active_desktop_changed();

private:
    xcb_connection_t* xcb_;
    xcb_window_t root_;
};



class X11_notifier : public QObject {
    Q_OBJECT
public:
    static X11_notifier* get();

    void output();
    QRect get_current_screen();

signals:
    void current_screen_changed(const QRect& screen);

private slots:
    void active_desktop_changed();

private:
    X11_notifier();
    ~X11_notifier();

    void update_monitors();
    void update_desktop();

    void stop_loop();

    std::vector<monitor> monitors;
    unsigned current_desktop_index;
    int current_monitor_index;

    xcb_connection_t* xcb;
    xcb_window_t root;
    xcb_ewmh_connection_t* ewmh;

    inline static X11_notifier* instance = nullptr;


    X11_loop* loop;
    QThread loop_thread;
};
