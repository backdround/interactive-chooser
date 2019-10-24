#include <QApplication>
#include <QThread>

#include "window_placer.h"
#include "x11_notifier.h"

Window_placer::Window_placer(QWindow* window) : managed_window_(window) {
    connect(X11_notifier::get_instance(),
            &X11_notifier::current_monitor_changed, this,
            &Window_placer::update_window_geometry);
    auto geometry =
        X11_notifier::get_instance()->get_current_monitor_geometry();
    update_window_geometry(geometry);
}

void Window_placer::update_window_geometry(const QRect& geometry) {
    auto width_k = 0.4;
    auto height_k = 0.9;

    auto width = geometry.width() * width_k;
    auto height = geometry.height() * height_k;
    auto x = geometry.x() + geometry.width() * (0.5 - width_k * 0.5);
    auto y = geometry.y() + geometry.height() * (0.5 - height_k * 0.5);

    managed_window_->setGeometry(x, y, width, height);
}
