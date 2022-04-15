QT       += core gui

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
    camProjection.cpp \
    cubeobject.cpp \
    functions.cpp \
    lights.cpp \
    lodepng/lodepng.cpp \
    main.cpp \
    mainwindow.cpp \
    raster.cpp \
    renderWindow.cpp \
    shading.cpp

HEADERS += \
    camProjection.h \
    cubeobject.h \
    functions.h \
    lights.h \
    lodepng/lodepng.h \
    mainwindow.h \
    raster.h \
    renderWindow.h \
    shading.h \
    stb_image.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../assimpF/assimp/bin/release/ -lassimp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../assimpF/assimp/bin/debug/ -lassimp
else:unix: LIBS += -L$$PWD/../../../assimpF/assimp/bin/ -lassimp

INCLUDEPATH += $$PWD/../../../assimpF/assimp/include
DEPENDPATH += $$PWD/../../../assimpF/assimp/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../assimp-source/assimp/bin/release/ -lassimp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../assimp-source/assimp/bin/debug/ -lassimp
else:unix: LIBS += -L$$PWD/../../../assimp-source/assimp/bin/ -lassimp

INCLUDEPATH += $$PWD/../../../assimp-source/assimp/include
DEPENDPATH += $$PWD/../../../assimp-source/assimp/include

DISTFILES += \
    check.png
