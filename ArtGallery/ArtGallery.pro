QT       += core gui opengl
LIBS += -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    functions.cpp \
    light.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_scene.cpp \
    mainwindow_setShader.cpp \
    mainwindow_shadowMap.cpp \
    object.cpp \


HEADERS += \
    functions.h \
    light.h \
    mainwindow.h \
    object.h \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/assimp/bin/release/ -lassimp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/assimp/bin/debug/ -lassimp
else:unix: LIBS += -L$$PWD/assimp/bin/ -lassimp

INCLUDEPATH += $$PWD/assimp/include
DEPENDPATH += $$PWD/assimp/include

DISTFILES += \
    shaders/cubemapEnv.frag \
    shaders/cubemapEnv.vert \
    shaders/depthMap.frag \
    shaders/depthMap.vert \
    shaders/depthMap_quad.frag \
    shaders/depthMap_quad.vert \
    shaders/reflection.frag \
    shaders/reflection.vert \
    shaders/shader.frag \
    shaders/shader.vert
