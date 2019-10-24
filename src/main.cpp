#include <QGuiApplication>
#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QRect>

#include <QQmlEngine>
#include <QQuickWindow>
#include <QQmlComponent>
#include <QQmlContext>

#include "focused_screen.h"
#include "x11_notifier.h"


void update_widget_geometry(QQuickWindow* widget, const QRect& screen_geometry) {
    auto new_width = screen_geometry.width() / 2;
    auto new_height = screen_geometry.height() / 2;
    auto new_x = screen_geometry.x() + screen_geometry.width() / 4;
    auto new_y = screen_geometry.y() + screen_geometry.height() / 4;

    QRect geometry(new_x, new_y, new_width, new_height);

    widget->setGeometry(geometry);
}

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QQmlEngine engine;
    QQmlComponent component(&engine, ":/main.qml");
    auto object = component.create();

    if (!object) {
        qCritical() << "Failed to create main component:";
        qCritical() << component.errors();
        return -1;
    }
    object->setParent(&component);


    auto window = qobject_cast<QQuickWindow*>(object);
    //connect(window, &QQuickWindow::cl)

    Focused_screen focused_screen;
    QObject::connect(&focused_screen, &Focused_screen::geometry_changed,
                     [&window](const QRect& screen_geometry) {
        update_widget_geometry(window, screen_geometry);
    });
    update_widget_geometry(window, focused_screen.geometry());
    window->show();

    return app.exec();
}
