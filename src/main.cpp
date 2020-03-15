#include <QGuiApplication>
#include <QDebug>
#include <QObject>

#include <QQmlEngine>


#include "window_placer.h"
#include "model_factory.h"
#include "main_window.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    Model_factory factory(argc, argv);
    auto model = factory.create();

    if (!model) {
        qCritical() << "Couldn't create model";
        exit(-1);
    }

    QQmlEngine engine;

    Main_window main_window(engine, *model);
    auto window = main_window.quick_window();

    if (!window) {
        qCritical() << "Couldn't create window";
        exit(-1);
    }

    Window_placer placer(window);
    window->show();

    return app.exec();
}
