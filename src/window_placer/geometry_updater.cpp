#include <QDebug>
#include <QTimer>

#include "geometry_updater.h"

Geometry_updater::Geometry_updater(QWindow& window, Monitor_geometry& geometry)
    : managed_window_(window) {
    connect(&geometry, &Monitor_geometry::current_monitor_changed, this,
            &Geometry_updater::update_window_geometry);
    update_window_geometry(geometry.current_monitor_geometry());
}

void Geometry_updater::update_window_geometry(const QRect& geometry) {
    // Disable focus accept.
    managed_window_.setFlag(Qt::WindowDoesNotAcceptFocus, true);

    // Set relative_unit to qml object.
    auto relative_unit = managed_window_.property("relative_unit");
    if (!relative_unit.isNull()) {
        auto value = QVariant::fromValue(geometry.height() / 10);
        managed_window_.setProperty("relative_unit", value);
    } else {
        qWarning() << "Couldn't set relative_unit";
    }


    // Set window geometry.
    auto width_k = 0.4;
    auto height_k = 0.9;

    auto width = geometry.width() * width_k;
    auto height = geometry.height() * height_k;
    auto x = geometry.x() + geometry.width() * (0.5 - width_k * 0.5);
    auto y = geometry.y() + geometry.height() * (0.5 - height_k * 0.5);

    managed_window_.setGeometry(x, y, width, height);

    // Enable focus accept.
    managed_window_.setFlag(Qt::WindowDoesNotAcceptFocus, false);
    QTimer::singleShot(10, [this] {
        managed_window_.requestActivate();
    });
}
