TARGET = GPPrimitiveEngine
TEMPLATE = app
CONFIG -= windows
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG -= core

win32 {
    QMAKE_LFLAGS += -Wl,-subsystem,windows

    INCLUDEPATH += include

    LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfgui
}
!win32 {
    INCLUDEPATH += /usr/local/include/ include

    LIBS += -L/usr/lib/x86_64-linux-gnu -L/usr/local/lib/ -ljpeg -lsfml-graphics -lsfml-window -lsfml-system -lsfgui
}

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}

QMAKE_CXXFLAGS += -fpermissive
QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    src/ValuesManager.cpp \
    src/utils.cpp \
    src/TexturesManager.cpp \
    src/State.cpp \
    src/main.cpp \
    src/List.cpp \
    src/cJSON.cpp \
    src/Application.cpp \
    src/StateMain.cpp \
    src/Engine3D/Vector2D.cpp \
    src/Engine3D/Vector3D.cpp \
    src/Engine3D/Matrix.cpp \
    src/Engine3D/Color.cpp \
    src/Engine3D/Point3D.cpp \
    src/Engine3D/Plane3D.cpp \
    src/Engine3D/Line3D.cpp \
    src/Engine3D/Triangle3D.cpp \
    src/Engine3D/Light.cpp \
    src/Engine3D/Camera.cpp \
    src/Engine3D/Engine3D.cpp \
    src/Engine3D/Primitive3D.cpp \
    src/Engine3D/ExtVector3D.cpp \
    src/Engine3D/Object3D.cpp \
    src/MemoryPool.cpp

HEADERS += \
    include/ValuesManager.h \
    include/utils.h \
    include/TexturesManager.h \
    include/State.h \
    include/List.h \
    include/cJSON.h \
    include/Application.h \
    include/StateMain.h \
    include/Engine3D/Vector2D.h \
    include/Engine3D/Vector3D.h \
    include/Engine3D/Matrix.h \
    include/Engine3D/Color.h \
    include/Engine3D/Point3D.h \
    include/Engine3D/Line3D.h \
    include/Engine3D/Plane3D.h \
    include/Engine3D/Triangle3D.h \
    include/Engine3D/Light.h \
    include/Engine3D/Camera.h \
    include/Engine3D/Engine3D.h \
    include/Engine3D/Primitive3D.h \
    include/Engine3D/ExtVector3D.h \
    include/Engine3D/Object3D.h \
    include/MemoryPool.h
