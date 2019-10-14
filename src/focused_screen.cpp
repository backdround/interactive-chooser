#include <QApplication>
#include <QThread>

#include "focused_screen.h"
#include "x11_notifier.h"

Focused_screen::Focused_screen() {
    connect(X11_notifier::get(), &X11_notifier::current_screen_changed, this,
            [this](const QRect& screen_rect) { set_geometry(screen_rect); });
}

QRect Focused_screen::geometry() {
    return geometry_;
}

void Focused_screen::set_geometry(const QRect& new_geometry) {
    geometry_ = new_geometry;
    emit geometry_changed(geometry_);
}
