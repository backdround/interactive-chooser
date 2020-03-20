#include <QDebug>
#include <QMetaObject>

#include "geometry_updater.h"

Geometry_updater::Geometry_updater(QWindow& window, Monitor_geometry& geometry)
    : managed_window_(window) {
    connect(&geometry, &Monitor_geometry::current_monitor_changed, this,
            &Geometry_updater::update_window_geometry);
    update_window_geometry(geometry.current_monitor_geometry());
}

void Geometry_updater::update_window_geometry(const QRect& geometry) {
    // Set unit_k to qml object.
    auto unit_k = managed_window_.property("unit_k");
    if (!unit_k.isNull()) {
        auto value = QVariant::fromValue(geometry.height() / 10);
        managed_window_.setProperty("unit_k", value);
    } else {
        qWarning() << "Couldn't set unit_k";
    }


    // Set window geometry.
    auto width_k = 0.4;
    auto height_k = 0.9;

    auto width = geometry.width() * width_k;
    auto height = geometry.height() * height_k;
    auto x = geometry.x() + geometry.width() * (0.5 - width_k * 0.5);
    auto y = geometry.y() + geometry.height() * (0.5 - height_k * 0.5);

    managed_window_.setGeometry(x, y, width, height);
}
