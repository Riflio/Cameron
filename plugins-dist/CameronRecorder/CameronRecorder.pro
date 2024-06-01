QT -= gui
QT += network

TARGET = CameronRecorder
TEMPLATE = lib

DEFINES += CAMERONRECORDER_LIBRARY
DEFINES += QT_MESSAGELOGCONTEXT

SOURCES += cameronrecorder.cpp \
    recorder.cpp \
    recorderthread.cpp

HEADERS += cameronrecorder.h \
    recordersettings.h \
    recorderthread.h \
    recorder.h \
    interfaces/icameronrecorder.h \
    interfaces/irecorder.h \
    interfaces/recfileinfo.h \
    interfaces/iplugininterface.h \
    interfaces/istreamer.h \
    interfaces/icameron.h \
    interfaces/iqcameron.h \
    interfaces/iqsettings.h \
    interfaces/icameronstream.h \
    interfaces/iqrtspstream.h \
    interfaces/irtppacketh264.h \
    interfaces/irtppacketh264unitfu.h

DISTFILES += \
    cameronrecorder.json
unix {
    target.path = /usr/lib
    INSTALLS += target
}
