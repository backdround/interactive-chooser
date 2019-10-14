#pragma once
#include <QGuiApplication>
#include <QCursor>
#include <QScreen>

class Focused_screen : public QObject {
    Q_OBJECT
    Q_PROPERTY(
        QRect geometry READ geometry WRITE set_geometry NOTIFY geometry_changed)

public:
    Focused_screen();

    QRect geometry();
    void set_geometry(const QRect& new_geometry);

signals:
    void geometry_changed(const QRect& width);

private:
    QRect geometry_;
};

