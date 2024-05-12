TEMPLATE = app

QT += network core
QT -= gui

CONFIG += c++11
CONFIG -= app_bundle
CONFIG += console


SOURCES += main.cpp \
    rtp/rtp_packet_h264.cpp \
    rtp/rtp_packet_h264_unit_fu.cpp \
    rtp/rtp_packet_h265.cpp \
    rtp/rtp_packet_h265_unit_fu.cpp \
    rtsp/rtsp.cpp \
    rtsp/rtsp_channel.cpp \
    rtsp/rtsp_stream.cpp \
    sdp/sdp.cpp \
    settings/settings.cpp \
    appcore.cpp \
    server/server.cpp \
    server/server_client.cpp \
    camera/cameras.cpp \
    camera/cameras_camera.cpp \
    server/server_client_streamer.cpp \
    plugins/eventer.cpp \
    plugins/pluginsmanager.cpp \
    plugins/dynamicqobject.cpp \
    rtp/rtp_packet.cpp \
    server/server_client_info.cpp

HEADERS += \
    interfaces/IBuffer.h \
    plugins/defines.h \
    plugins/wpropertyinterface.h \
    estd/estd.h \
    rtp/rtp_packet_h264.h \
    rtp/rtp_packet_h264_unit_fu.h \
    rtp/rtp_packet_h265.h \
    rtp/rtp_packet_h265_unit_fu.h \
    rtsp/rtsp.h \
    rtsp/rtsp_channel.h \
    rtsp/rtsp_stream.h \
    sdp/sdp.h \
    settings/settings.h \
    appcore.h \
    server/server.h \
    server/server_client.h \
    camera/cameras.h \
    camera/cameras_camera.h \
    server/server_client_streamer.h \
    plugins/eventer.h \
    plugins/PluginEventsWrapper.h \
    plugins/PluginInterface.h \
    plugins/pluginsmanager.h \
    plugins/dynamicqobject.h \
    plugins/usingleton.h \
    interfaces/icameras_camera.h \
    interfaces/iserver.h \
    interfaces/icameras.h \
    interfaces/isdp.h \
    interfaces/irtsp_stream.h \
    rtp/rtp_packet.h \
    interfaces/irtp_packet.h \
    server/server_client_info.h

DEFINES += QT_MESSAGELOGCONTEXT

QT += core-private network-private


#QMAKE_LFLAGS += -static-libstdc++
#QMAKE_CXXFLAGS += -static
#QMAKE_LFLAGS += -static
