#ifndef H264_DECODER_H
#define H264_DECODER_H

#include <QObject>
#include <QSharedPointer>

#include <QGuiApplication>

#include "../rtsp/rtsp_stream.h"



namespace NS_H264 {
extern "C"
{
#include <../../ffmpeg/include/libavcodec/avcodec.h>
#include <../../ffmpeg/include/libavformat/avformat.h>
#include <../../ffmpeg/include/libavfilter/avfilter.h>
#include <../../ffmpeg/include/libswscale/swscale.h>
#include <../../ffmpeg/include/libavutil/frame.h>
#include <../../ffmpeg/include/libavutil/common.h>
}



class H264_Decoder : public QObject
{
    Q_OBJECT
public:
    explicit H264_Decoder(QObject *parent, NS_RSTP::RTSP_Stream * streamer);

    bool init();

signals:
    void newDecodedFrameData(QSharedPointer<uint8_t> data, int width, int height);

public slots:    
    void process();

private:
    AVCodec* m_decoder;
    AVCodecContext* m_decoderContext;
    AVFrame* m_picture;
    AVPacket m_packet;

    NS_RSTP::RTSP_Stream * _streamer;

    int _framesOffset;

};
}




#endif // H264_DECODER_H
