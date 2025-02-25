QT += widgets sql

TEMPLATE =  lib
DEFINES += TESTERPLUGIN_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    testerplugin.cpp

HEADERS += \
    TesterPlugin_global.h \
    testerplugin.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/release/ -lLauncherPluginBase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/debug/ -lLauncherPluginBase
else:unix: LIBS += -L$$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/ -lLauncherPluginBase

INCLUDEPATH += $$PWD/../LauncherPluginBase
DEPENDPATH += $$PWD/../LauncherPluginBase

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/release/libLauncherPluginBase.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/debug/libLauncherPluginBase.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/release/LauncherPluginBase.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/debug/LauncherPluginBase.lib
else:unix: PRE_TARGETDEPS += $$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/libLauncherPluginBase.a
