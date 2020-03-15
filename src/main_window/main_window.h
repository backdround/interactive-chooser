#pragma once
#include <QObject>

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQmlComponent>

#include "model_interface.h"
#include "viewmodel.h"

class Main_window : public QObject {
    Q_OBJECT

public:
    Main_window(QQmlEngine& engine, Model_interface& model);

    QQuickWindow* quick_window();

private:
    QQuickWindow* window_ = nullptr;
    Viewmodel viewmodel_;
};
