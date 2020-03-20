#pragma once
#include <QObject>
#include <QRect>

class Monitor_geometry : public QObject {
    Q_OBJECT

public:
    virtual QRect current_monitor_geometry() = 0;

signals:
    void current_monitor_changed(const QRect& geometry);
};
