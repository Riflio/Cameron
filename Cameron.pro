TEMPLATE = app

QT += network core
QT -= gui

CONFIG += c++11
CONFIG -= app_bundle
CONFIG += console


SOURCES += main.cpp \
    camera/camera.cpp \
    rtp/rtppacket.cpp \
    rtp/rtppacketh264.cpp \
    rtp/rtppacketh264unitfu.cpp \
    rtp/rtppacketh265.cpp \
    rtp/rtppacketh265unitfu.cpp \
    rtsp/rtsp.cpp \
    rtsp/rtspchannel.cpp \
    rtsp/rtspstream.cpp \
    sdp/sdp.cpp \
    server/serverclient.cpp \
    server/serverclientinfo.cpp \
    server/serverclientstreamer.cpp \
    settings/settings.cpp \
    appcore.cpp \
    server/server.cpp \
    camera/cameras.cpp \
    plugins/pluginsmanager.cpp

HEADERS += \
    camera/camera.h \
    estd/estd.h \
    interfaces/icamera.h \
    interfaces/icameron.h \
    interfaces/iplugininterface.h \
    interfaces/ipluginsmanager.h \
    interfaces/iqcamera.h \
    interfaces/iqcameron.h \
    interfaces/iqrtspstream.h \
    interfaces/iqsettings.h \
    interfaces/irtppacket.h \
    interfaces/irtppacketh264.h \
    interfaces/irtppacketh264unitfu.h \
    interfaces/irtppacketh265.h \
    interfaces/irtppacketh265unitfu.h \
    interfaces/irtspstream.h \
    interfaces/iserverclient.h \
    interfaces/iserverclientinfo.h \
    interfaces/isettings.h \
    rtp/rtppacket.h \
    rtp/rtppacketh264.h \
    rtp/rtppacketh264unitfu.h \
    rtp/rtppacketh265.h \
    rtp/rtppacketh265unitfu.h \
    rtsp/rtsp.h \
    rtsp/rtspchannel.h \
    rtsp/rtspstream.h \
    sdp/sdp.h \
    server/serverclient.h \
    server/serverclientinfo.h \
    server/serverclientstreamer.h \
    settings/settings.h \
    appcore.h \
    server/server.h \
    camera/cameras.h \
    plugins/pluginsmanager.h \
    interfaces/iserver.h \
    interfaces/icameras.h \
    interfaces/isdp.h

DEFINES += QT_MESSAGELOGCONTEXT

QT += core-private network-private


#QMAKE_LFLAGS += -static-libstdc++
#QMAKE_CXXFLAGS += -static
#QMAKE_LFLAGS += -static
