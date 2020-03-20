#pragma once
#include <QWindow>
#include <QObject>

#include "monitor_geometry.h"

class Geometry_updater : public QObject {
    Q_OBJECT

public:
    Geometry_updater(QWindow& window, Monitor_geometry& geometry);

private slots:
    void update_window_geometry(const QRect& screen_rect);

private:
    QWindow& managed_window_;
};
