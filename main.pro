TEMPALTE = app
QT += core gui widgets qml quick
CONFIG += debug c++17
SOURCES += src/main.cpp src/focused_screen.cpp src/x11_notifier.cpp
HEADERS += src/focused_screen.h src/x11_notifier.h
RESOURCES += resources.qrc
LIBS += -lxcb -lxcb-ewmh -lxcb-randr

