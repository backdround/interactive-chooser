#include <QGuiApplication>
#include <QObject>
#include <QRect>

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>

#include "focused_screen.h"
#include "x11_notifier.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    Focused_screen focused_screen;

    QQmlEngine engine;
    engine.rootContext()->setContextProperty("focused_screen", &focused_screen);
    QQmlComponent component(&engine, ":/main.qml");
    auto object = component.create();
    object->setParent(&component);

    return app.exec();
}
