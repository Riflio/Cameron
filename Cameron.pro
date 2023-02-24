TEMPLATE = app

QT += network core
QT -= gui

CONFIG += c++11
CONFIG -= app_bundle
CONFIG += console


SOURCES += main.cpp \
    Plugins/wthread.cpp \
    rtp/rtp_packet_h264.cpp \
    rtp/rtp_packet_h264_unit_fu.cpp \
    rtsp/rtsp.cpp \
    rtsp/rtsp_channel.cpp \
    rtsp/rtsp_stream.cpp \
    sdp/sdp.cpp \
    settings/settings.cpp \
    appcore.cpp \
    Server/server.cpp \
    Server/server_client.cpp \
    Camera/cameras.cpp \
    Camera/cameras_camera.cpp \
    Server/server_client_streamer.cpp \
    Plugins/eventer.cpp \
    Plugins/pluginsmanager.cpp \
    Plugins/dynamicqobject.cpp \
    rtp/rtp_packet.cpp \
    Server/server_client_info.cpp

HEADERS += \
    Interfaces/IBuffer.h \
    Plugins/defines.h \
    Plugins/wpropertyinterface.h \
    rtp/rtp_packet_h264.h \
    rtp/rtp_packet_h264_unit_fu.h \
    rtsp/rtsp.h \
    rtsp/rtsp_channel.h \
    rtsp/rtsp_stream.h \
    sdp/sdp.h \
    settings/settings.h \
    appcore.h \
    Server/server.h \
    Server/server_client.h \
    Camera/cameras.h \
    Camera/cameras_camera.h \
    Server/server_client_streamer.h \
    Plugins/eventer.h \
    Plugins/PluginEventsWrapper.h \
    Plugins/PluginInterface.h \
    Plugins/pluginsmanager.h \
    Plugins/dynamicqobject.h \
    Plugins/usingleton.h \
    Interfaces/icameras_camera.h \
    Interfaces/iserver.h \
    Interfaces/icameras.h \
    Interfaces/isdp.h \
    Interfaces/irtsp_stream.h \
    Interfaces/ithread.h \
    rtp/rtp_packet.h \
    Interfaces/irtp_packet.h \
    Plugins/wthread.h \
    Server/server_client_info.h \
    Interfaces/ithread.h

DEFINES += QT_MESSAGELOGCONTEXT



#QMAKE_LFLAGS += -static-libstdc++
#QMAKE_CXXFLAGS += -static
#QMAKE_LFLAGS += -static
