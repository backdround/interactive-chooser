#pragma once
#include <QWindow>

class Window_placer : public QObject {
    Q_OBJECT

public:
    Window_placer(QWindow* window);

private slots:
    void update_window_geometry(const QRect& screen_rect);

private:
    QWindow* managed_window_;
};
