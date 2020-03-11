#include <QGuiApplication>
#include <QObject>

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQmlComponent>

#include "window_placer.h"
#include "model_factory.h"
#include "viewmodel.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    Model_factory factory(argc, argv);
    auto model = factory.create();

    Viewmodel viewmodel(*model);

    QQmlEngine engine;
    engine.rootContext()->setContextProperty("_model", &viewmodel);
    QQmlComponent component(&engine, ":/main.qml");
    auto object = component.create();

    if (!object) {
        qCritical() << "Failed to create main component:";
        qCritical() << component.errors();
        return -1;
    }
    object->setParent(&component);

    auto window = qobject_cast<QQuickWindow*>(object);
    Window_placer placer(window);
    window->show();

    return app.exec();
}
