
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../assimpF/assimp/bin/release/ -lassimp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../assimpF/assimp/bin/debug/ -lassimp
else:unix: LIBS += -L$$PWD/../../assimpF/assimp/bin/ -lassimp

INCLUDEPATH += $$PWD/../../assimpF/assimp/include
DEPENDPATH += $$PWD/../../assimpF/assimp/include

HEADERS += \
    UTFConverter.h

SOURCES += \
    main.cpp
