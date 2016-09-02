#ifndef H264_STREAM_RTP_H
#define H264_STREAM_RTP_H

#include <QObject>

#include "../rtsp/rtsp_stream.h"
#include "../rtp/rtp.h"
#include "h264_fuheader.h"

/**
 * @brief Формируем h264 фреймы из принятых rtp данных
 * В каждом RTP пакете идёт NAL блок (или его часть т.к. целиком NAL в один пакет не влезает)
 * наша задача собрать NAL блок и засунуть итоговый фрейм в RTSP_Stream
 * что бы потом уже другие решали, что делать с готовым
 *
 * Спека: http://ip.hhi.de/imagecom_G1/assets/pdfs/h264_iso-iec_14496-10.pdf
 */

namespace NS_H264 {


class H264_Stream_RTP : public NS_RSTP::RTSP_Stream
{
    Q_OBJECT
public:

    enum NAL_FLAGS {
        FU_A = 28,
        FU_B = 29,
        SPS = 7,
        PPS = 8,
        IDR = 5,
        NON_IDR = 1
    };

    const QByteArray NAL_DELEMITER = QByteArray::fromRawData("\x00\x00\x00\x01", 4); //-- разделитель NAL блоков 00 00 00 01


    explicit H264_Stream_RTP(NS_RSTP::RTSP_Stream *parent);

    void setPS(QString sps, QString pps);

    QByteArray getPS();

signals:   
    void finished();

public slots:
    bool parse();
    bool process();


private:
    BYTE getReconstructedNal();
    H264_FUHeader * getFUHeader();

    int getH264PayloadStart();
    int getH264PayloadLength();

    QByteArray _frame;

    QByteArray _sps;
    QByteArray _pps;

    H264_FUHeader * _fuHeader;
    BYTE _nal;

    BYTE _nalType;
};
}
#endif // H264_STREAM_RTP_H
