#include "h264_decoder.h"
#include <QDebug>
namespace NS_H264 {


H264_Decoder::H264_Decoder(QObject *parent, NS_RSTP::RTSP_Stream * streamer) : QObject(parent)
{    
    _streamer = streamer;
    _framesOffset = -1;

    qRegisterMetaType< QSharedPointer<uint8_t> >("QSharedPointer<uint8_t>");
}

bool H264_Decoder::init()
{
    av_register_all();
    av_init_packet(&m_packet);
    m_decoder = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!m_decoder) {
        qDebug()<<"Cannot find H264 codec!";
        return false;
    }
    m_decoderContext = avcodec_alloc_context3(m_decoder);

    if (m_decoder->capabilities & CODEC_CAP_TRUNCATED) {
        m_decoderContext->flags |= CODEC_FLAG_TRUNCATED;

    }

    m_decoderContext->flags2 |= CODEC_FLAG2_CHUNKS;
    m_decoderContext->thread_count = 1;//TODO: random value. May be changing can make decoding faster

    AVDictionary* dictionary = nullptr;
    if (avcodec_open2(m_decoderContext, m_decoder, &dictionary) < 0) {
        qDebug()<<"Failed to open decoder!";
        return false;
    }

    qDebug() << "H264 Decoder successfully opened";
    m_picture = av_frame_alloc();

    return true;
}

/**
 * @brief декодируем, пока не лень
 */
void H264_Decoder::process()
{

    while(1==1) {
        QGuiApplication::processEvents();
        QByteArray frame = _streamer->getFrame(_framesOffset);

        if (frame.isEmpty()) continue;

        //qDebug()<<"TIMESTAMP"<<_streamer->getTimeStamp();

        //qDebug() << "H264Decoder: received encoded frame with framesize " << frame.length();

        m_packet.size = frame.size();
        m_packet.data = reinterpret_cast<uint8_t*>( frame.data() );

        while(m_packet.size > 0) {
            QGuiApplication::processEvents();
            int got_picture;
            int len = avcodec_decode_video2(m_decoderContext, m_picture, &got_picture, &m_packet);

            if (len < 0) {
                qDebug() << "Decoding error";                
                break;
            }


            if (_streamer->getOffset(_framesOffset)< 10 &&   got_picture) {

                if ( m_picture->format == AV_PIX_FMT_YUV420P ) {
                    static SwsContext * m_swsCtx = NULL;

                    m_swsCtx = sws_getCachedContext (m_swsCtx, m_picture->width, m_picture->height, AV_PIX_FMT_YUV420P, m_picture->width, m_picture->height, AV_PIX_FMT_RGB24, SWS_GAUSS, NULL, NULL, NULL );

                    int numBytes=avpicture_get_size(AV_PIX_FMT_RGB24, m_decoderContext->width, m_decoderContext->height);
                    AVFrame * pFrameRGB = av_frame_alloc();

                    QSharedPointer<uint8_t> buffer ( (uint8_t *)av_malloc(numBytes*sizeof(uint8_t)) );

                    avpicture_fill((AVPicture *)pFrameRGB, buffer.data(), AV_PIX_FMT_RGB24, m_decoderContext->width, m_decoderContext->height);

                    if (sws_scale ( m_swsCtx, m_picture->data, m_picture->linesize, 0, m_picture->height, pFrameRGB->data,  pFrameRGB->linesize) != m_picture->height ) {
                        qDebug() << "PIZDETS!!!";
                        continue;
                    }

                    emit newDecodedFrameData(buffer, m_picture->width, m_picture->height);

                    av_free(pFrameRGB);

                }
            }

            m_packet.size -= len;
            m_packet.data += len;
        }
        QGuiApplication::processEvents();

    }
}




}
