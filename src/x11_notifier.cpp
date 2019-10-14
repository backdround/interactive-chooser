#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <iostream>
using namespace std;

#include "x11_notifier.h"

void X11_loop::run() {
    const uint32_t input_event_filter[] = {XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                                           XCB_EVENT_MASK_PROPERTY_CHANGE};

    xcb_change_window_attributes(xcb_, root_, XCB_CW_EVENT_MASK,
                                 input_event_filter);
    xcb_flush(xcb_);

    auto capture_event_name = "_NET_CURRENT_DESKTOP";
    while (auto event = xcb_wait_for_event(xcb_)) {
        if ((event->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
            cout << "close event lol" << endl;
            break;
        } else if (event->response_type == XCB_PROPERTY_NOTIFY) {
            auto property_event =
                reinterpret_cast<xcb_property_notify_event_t*>(event);
            auto name_cookie = xcb_get_atom_name(xcb_, property_event->atom);
            auto name_reply =
                xcb_get_atom_name_reply(xcb_, name_cookie, nullptr);
            auto atom_name = xcb_get_atom_name_name(name_reply);

            if (memcmp(atom_name, capture_event_name, name_reply->name_len) ==
                0) {
                cout << "current desktop changed" << endl;
                emit active_desktop_changed();
            }
            free(name_reply);
        } else {
            cout << "unknown event" << endl;
        }

        free(event);
    }
}


X11_notifier* X11_notifier::get() {
    if (!instance) {
        instance = new X11_notifier();
        qAddPostRoutine([] {
            delete instance;
        });
    }
    return instance;
}

X11_notifier::X11_notifier() {
    // init x11
    xcb = xcb_connect(nullptr, nullptr);
    if (xcb_connection_has_error(xcb)) {
        cout << "Couldn't open xcb" << endl;
    }

    xcb_screen_t* screen = xcb_setup_roots_iterator(xcb_get_setup(xcb)).data;
    if (screen == nullptr) {
        cout << "Couldn't acquire screen" << endl;
    }

    root = screen->root;
    ewmh = new xcb_ewmh_connection_t;
    if (!xcb_ewmh_init_atoms_replies(ewmh, xcb_ewmh_init_atoms(xcb, ewmh),
                                     nullptr)) {
        cout << "Couldn't initialize EWMH atoms.\n" << endl;
    }

    // init desktop
    update_monitors();
    update_desktop();

    // init thread
    loop = new X11_loop(xcb, root);
    connect(loop, &X11_loop::active_desktop_changed, this,
            &X11_notifier::active_desktop_changed);
    loop->moveToThread(&loop_thread);
    connect(&loop_thread, &QThread::started, loop, &X11_loop::run);
    loop_thread.start();
}

X11_notifier::~X11_notifier() {
    instance = nullptr;

    // stop thread
    stop_loop();
    loop_thread.quit();
    loop_thread.wait();

    // realize resources
    xcb_ewmh_connection_wipe(ewmh);
    delete ewmh;
    xcb_disconnect(xcb);
}

void X11_notifier::active_desktop_changed() {
    update_desktop();
    auto& monitor = monitors[current_monitor_index];
    emit current_screen_changed(
        {monitor.x, monitor.y, monitor.width, monitor.height});
}

void X11_notifier::update_monitors() {
    auto get_monitors_cookie = xcb_randr_get_monitors(xcb, root, true);
    auto get_monitors_reply =
        xcb_randr_get_monitors_reply(xcb, get_monitors_cookie, nullptr);

    if (!get_monitors_reply) {
        cout << "Couldn't get monitors" << endl;
        return;
    }

    int monitors_count =
        xcb_randr_get_monitors_monitors_length(get_monitors_reply);
    auto monitors_iterator =
        xcb_randr_get_monitors_monitors_iterator(get_monitors_reply);

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
    // get current desktop index
    current_desktop_index = XCB_NONE;
    auto query = xcb_ewmh_get_current_desktop(ewmh, 0);
    if (!xcb_ewmh_get_current_desktop_reply(ewmh, query, &current_desktop_index,
                                            nullptr)) {
        cout << "Couldn't get current_desktop_index desktop" << endl;
        return;
    }

    // viewports
    auto get_desktop_viewport_cookie = xcb_ewmh_get_desktop_viewport(ewmh, 0);
    xcb_ewmh_get_desktop_viewport_reply_t viewport;
    if (!xcb_ewmh_get_desktop_viewport_reply(ewmh, get_desktop_viewport_cookie,
                                             &viewport, nullptr)) {
        cout << "Couldn't get desktop viewport" << endl;
        return;
    }

    for (int i = 0; i < viewport.desktop_viewport_len; i++) {
        int viewport_x = viewport.desktop_viewport[i].x;
        int viewport_y = viewport.desktop_viewport[i].y;

        if (i == current_desktop_index) {
            for (int i = 0; i < monitors.size(); i++) {
                int monitor_x = monitors[i].x;
                int monitor_y = monitors[i].y;
                if ((monitor_x == viewport_x) && (monitor_y == viewport_y)) {
                    current_monitor_index = i;
                    break;
                }
            }
            break;
        }
    }
    xcb_ewmh_get_desktop_viewport_reply_wipe(&viewport);
}

void X11_notifier::stop_loop() {
    auto event = static_cast<xcb_client_message_event_t*>(
        calloc(1, sizeof(xcb_client_message_event_t)));

    event->response_type = XCB_CLIENT_MESSAGE;
    event->format = 32;
    event->sequence = 0;
    event->window = root;
    event->type = 0;
    const char* event_name = "CUSTOM_CLOSE_EVENT\0";
    memcpy(&event->data.data8, event_name, strlen(event_name));

    cout << "sending close event" << endl;
    xcb_send_event(xcb, false, root, XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
                   reinterpret_cast<const char*>(event));

    xcb_flush(xcb);
}

void X11_notifier::output() {
    cout << "desktop index, viewport index : " << current_desktop_index << ", "
         << current_desktop_index << endl;
    auto& monitor = monitors[current_monitor_index];
    cout << monitor.x << "x" << monitor.y << ":" << monitor.width << "x"
         << monitor.height << endl;
}
