TEMPALTE = app
QT += core gui widgets qml quick quickwidgets
CONFIG += debug c++17

SOURCES += $$files(src/*.cpp) \
  $$files(src/x11/*.cpp)
HEADERS += $$files(src/*.h) \
  $$files(src/x11/*.h)

INCLUDEPATH *= src/x11/

RESOURCES += resources.qrc
LIBS += -lxcb -lxcb-ewmh -lxcb-randr

