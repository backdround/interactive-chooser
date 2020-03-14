#include "main_window.h"

Main_window::Main_window(QQmlEngine& engine, Model_interface& model)
    : viewmodel_(model) {

    engine.rootContext()->setContextProperty("_model", &viewmodel_);
    QQmlComponent component(&engine, "qrc:/main_window.qml");
    auto object = component.create();

    if (!object) {
        qCritical() << "Failed to create main component:";
        qCritical() << component.errors();
        return;
    }

    object->setParent(this);
    window_ = qobject_cast<QQuickWindow*>(object);
}

QQuickWindow* Main_window::quick_window() {
    return window_;
}
