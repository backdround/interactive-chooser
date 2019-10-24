#include <QDebug>
#include <QCoreApplication>
#include <QTimer>

#include "x11_event_loop.h"

#include "x11_notifier.h"

X11_notifier* X11_notifier::get_instance() {
    if (!instance) {
        instance = new X11_notifier();
        qAddPostRoutine([] {
            delete instance;
        });
    }
    return instance;
}

X11_notifier::X11_notifier() {
    // Init xcb.
    xcb = xcb_connect(nullptr, nullptr);
    if (xcb_connection_has_error(xcb)) {
        qWarning() << "Couldn't open xcb." << endl;
        return;
    }

    screen = xcb_setup_roots_iterator(xcb_get_setup(xcb)).data;
    if (screen == nullptr) {
        qWarning() << "Couldn't acquire screen." << endl;
        return;
    }

    ewmh = new xcb_ewmh_connection_t;
    if (!xcb_ewmh_init_atoms_replies(ewmh, xcb_ewmh_init_atoms(xcb, ewmh),
                                     nullptr)) {
        qWarning() << "Couldn't initialize EWMH atoms." << endl;
        return;
    }

    // Init desktop.
    get_monitors_info();
    update_desktop();

    // Init x11 event loop.
    loop = new X11_event_loop(xcb, screen);
    connect(loop, &X11_event_loop::active_desktop_changed, this,
            &X11_notifier::update_current_monitor);
    loop->moveToThread(&loop_thread);
    connect(&loop_thread, &QThread::started, loop, &X11_event_loop::run);
    loop_thread.start();
}

X11_notifier::~X11_notifier() {
    instance = nullptr;

    // Stop thread.
    loop->stop();
    loop_thread.quit();
    loop_thread.wait();

    // Realize resources.
    xcb_ewmh_connection_wipe(ewmh);
    delete ewmh;
    xcb_disconnect(xcb);
}


void X11_notifier::update_current_monitor() {
    update_desktop();
    emit current_monitor_changed(current_monitor);
}

QRect X11_notifier::get_current_monitor_geometry() {
    update_desktop();
    return current_monitor;
}

void X11_notifier::get_monitors_info() {
    // Get x11 monitors.
    auto get_monitors_cookie = xcb_randr_get_monitors(xcb, screen->root, true);
    auto get_monitors_reply =
        xcb_randr_get_monitors_reply(xcb, get_monitors_cookie, nullptr);

    if (!get_monitors_reply) {
        qWarning() << "Couldn't get monitors" << endl;
        return;
    }

    int monitors_count =
        xcb_randr_get_monitors_monitors_length(get_monitors_reply);
    auto monitors_iterator =
        xcb_randr_get_monitors_monitors_iterator(get_monitors_reply);

    // Save monitors size.
    for (int i = 0; i < monitors_count; i++) {
        int x = monitors_iterator.data->x;
        int y = monitors_iterator.data->y;
        int width = monitors_iterator.data->width;
        int height = monitors_iterator.data->height;

        monitors.push_back({x, y, width, height});
        xcb_randr_monitor_info_next(&monitors_iterator);
    }
    free(get_monitors_reply);
}

void X11_notifier::update_desktop() {
    // Get current desktop index.
    unsigned current_desktop_index = XCB_NONE;
    auto query = xcb_ewmh_get_current_desktop(ewmh, 0);
    if (!xcb_ewmh_get_current_desktop_reply(ewmh, query, &current_desktop_index,
                                            nullptr)) {
        qWarning() << "Couldn't get current_desktop_index" << endl;
        return;
    }

    // Get viewports.
    auto get_desktop_viewport_cookie = xcb_ewmh_get_desktop_viewport(ewmh, 0);
    xcb_ewmh_get_desktop_viewport_reply_t viewport;
    if (!xcb_ewmh_get_desktop_viewport_reply(ewmh, get_desktop_viewport_cookie,
                                             &viewport, nullptr)) {
        qWarning() << "Couldn't get desktop viewport" << endl;
        return;
    }

    // Sigfault check.
    if (viewport.desktop_viewport_len < current_desktop_index) {
        qWarning() << "Couldn't get viewport with current desktop" << endl;
        return;
    }

    // Get current_monitor.
    int viewport_x = viewport.desktop_viewport[current_desktop_index].x;
    int viewport_y = viewport.desktop_viewport[current_desktop_index].y;
    for (auto monitor : monitors) {
        if ((monitor.x() == viewport_x) && (monitor.y() == viewport_y)) {
            current_monitor = monitor;
            break;
        }
    }
    xcb_ewmh_get_desktop_viewport_reply_wipe(&viewport);
}
