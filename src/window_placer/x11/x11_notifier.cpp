#include <QDebug>

#include <cstring>

#include "x11_notifier.h"

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

    // Delegate event handling.
    auto desktop_changed_checker = [this](const xcb_generic_event_t* event) {
        QMetaObject::invokeMethod(this, "event_handler", Qt::DirectConnection,
                                  Q_ARG(const xcb_generic_event_t*, event));
    };

    // Init x11 event loop.
    loop_ = new X11_event_loop(xcb, screen, desktop_changed_checker);
}

X11_notifier::~X11_notifier() {
    delete loop_;

    // Realize resources.
    xcb_ewmh_connection_wipe(ewmh);
    delete ewmh;
    xcb_disconnect(xcb);
}

void X11_notifier::event_handler(const xcb_generic_event_t* event) {
    if (event->response_type != XCB_PROPERTY_NOTIFY) {
        return;
    }

    // Get atom name.
    auto property_event =
        reinterpret_cast<const xcb_property_notify_event_t*>(event);
    auto name_cookie = xcb_get_atom_name(xcb, property_event->atom);
    auto name_reply =
        xcb_get_atom_name_reply(xcb, name_cookie, nullptr);
    auto atom_name = xcb_get_atom_name_name(name_reply);

    // Compare with desktop changed atom name.
    auto desktop_changed_atom_name = "_NET_CURRENT_DESKTOP";
    if (memcmp(atom_name, desktop_changed_atom_name,
               name_reply->name_len) != 0) {
        return;
    }

    update_desktop();
    emit current_monitor_changed(current_monitor);
}

QRect X11_notifier::current_monitor_geometry() {
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
