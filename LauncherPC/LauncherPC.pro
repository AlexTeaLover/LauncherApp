QT       += core sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    launcher.cpp \
    main.cpp \
    authorization.cpp \
    processing.cpp

HEADERS += \
    authorization.h \
    launcher.h \
    processing.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Plugins
# else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Plugins

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-DBHandler-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/release/ -lDBHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-DBHandler-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/debug/ -lDBHandler

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-PaginationPlugin-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/release/ -lPaginationPlugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-PaginationPlugin-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/debug/ -lPaginationPlugin

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-TesterPlugin-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/release/ -lTesterPlugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-TesterPlugin-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/debug/ -lTesterPlugin

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/release/ -lLauncherPluginBase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/debug/ -lLauncherPluginBase

INCLUDEPATH += $$PWD/../LauncherPluginBase
DEPENDPATH += $$PWD/../LauncherPluginBase

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/release/libLauncherPluginBase.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/debug/libLauncherPluginBase.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/release/LauncherPluginBase.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-LauncherPluginBase-Desktop_Qt_6_6_0_MinGW_64_bit-Debug/debug/LauncherPluginBase.lib
