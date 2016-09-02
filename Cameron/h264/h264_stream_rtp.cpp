#include "h264_stream_rtp.h"
#include <QDebug>

#include <QGuiApplication>

namespace NS_H264 {

H264_Stream_RTP::H264_Stream_RTP(RTSP_Stream *parent) :
   NS_RSTP::RTSP_Stream(parent)
{

}

bool H264_Stream_RTP::process()
{
    _fuHeader = new H264_FUHeader();

    while(1==1) {
        QGuiApplication::processEvents();

        //-- берём следующий пакет из очереди RTP, если есть
        if (!getPacket())  continue;

        parse();

        QGuiApplication::processEvents();
    }

    emit finished();

    return true;
}

/**
 * @brief Устанавливаем базовые параметры картинки
 * @param pps
 * @param sps
 */
void H264_Stream_RTP::setPS(QString sps, QString pps)
{
    _sps = QByteArray::fromBase64(sps.toUtf8());
    _pps = QByteArray::fromBase64(pps.toUtf8());

    _sps.prepend(NAL_DELEMITER);
    _pps.prepend(NAL_DELEMITER);

    toNewFrame(_sps);
    toNewFrame(_pps);

}

/**
 * @brief отдаём кому надо SPS и PPS с разделителем
 * @return
 */
QByteArray H264_Stream_RTP::getPS()
{
    return _sps+_pps;
}


H264_FUHeader * H264_Stream_RTP::getFUHeader(){
    return _fuHeader;
}


int H264_Stream_RTP::getH264PayloadStart()
{
    switch(_nalType){
        case FU_A:
        case FU_B: {
            return getPayloadStart() + 2;
        }
        case SPS:
        case PPS: {
            return getPayloadStart();
        }
    }

    return 0;
}

int H264_Stream_RTP::getH264PayloadLength()
{
    switch(_nalType){
        case FU_A:
        case FU_B: {
            return getPayloadLength() - 2;
        }
        case SPS:
        case PPS: {
            return getPayloadLength();
        }
    }
    return 0;
}

/**
 * @brief фрейм h264 может не влезть в один пакет, поэтому нужно восстановить NAL заголовок для первого
 * берём первые 3 бита NAL и объединяем с пятью от fuHeader
 * @return
 */

BYTE H264_Stream_RTP::getReconstructedNal()
{
    BYTE nal = (BYTE)( (_nal & 0xE0) ^ _fuHeader->getType());

    return nal;
}

/**
 * @brief Принимаем данные и обрабатываем
 * @param data
 */
bool H264_Stream_RTP::parse()
{   

    _nal = _curPacket[getPayloadStart()] ; //-- берём первые 8 бит
    _fuHeader->set(_curPacket[getPayloadStart() + 1]);

    _nalType = _nal & 0x1f;

    static BYTE prevNalType = 0; //-- какой тип NAL был до этого
    static H264_FUHeader::FU_FLAG prevFUFlag = H264_FUHeader::FU_NONE; //-- смотрим, какой был предыдущий фрейм
    static bool fuBroken = false; //-- не удалось собрать фрейм

    switch (_nalType){ //-- первые два бита - тип NAL блока
        case FU_A: //-- если фрейм разбит на несколько пакетов
        case FU_B: {
            if(_fuHeader->isFirst()){ //-- первый фрейм
                //qDebug()<<"first";

                //--  если первый фрейм пошёл сразу после среднего, при этом предыдущий тип пакета то же был составным - проебали конец пакета, отправляем всё, что можем
                if ( !fuBroken && prevFUFlag == H264_FUHeader::FU_MIDDLE && (prevNalType!=FU_A && prevNalType!=FU_B) ) {
                    toNewFrame(_frame);
                }

                if (fuBroken) {
                    //qDebug()<<"prev packet is broken";
                }
                fuBroken = false;

                prevFUFlag = H264_FUHeader::FU_FIRST;
                prevNalType = _nalType;

                _frame.clear(); //-- начинаем собирать фрейм с чистого листа
                _frame.append(NAL_DELEMITER);
                _frame.append( getReconstructedNal() );
                _frame.append( _curPacket.mid( getH264PayloadStart(), getH264PayloadLength() ) );

            }  else if(_fuHeader->isMiddle()) {
                if ( fuBroken ) return false;
                if ( prevFUFlag == H264_FUHeader::FU_NONE ) return false; //-- ждём первый первый пакет при начале соединения
                if ( prevFUFlag == H264_FUHeader::FU_LAST || (prevNalType!=FU_A && prevNalType!=FU_B) ) { //-- проебали пакет с началом фрейма или посередине вклинился левый пакет, пизда всему фрейму
                    fuBroken = true;
                    return false;
                }

                //qDebug()<<"middle";
                prevFUFlag = H264_FUHeader::FU_MIDDLE;
                _frame.append( _curPacket.mid( getH264PayloadStart(), getH264PayloadLength() ) );

            }  else if(_fuHeader->isEnd()){

                if ( fuBroken ) return false;
                if ( prevFUFlag == H264_FUHeader::FU_NONE ) return false; //-- ждём первый первый пакет при начале соединения
                if ( prevNalType!=FU_A && prevNalType!=FU_B) { //-- посередине вклинился левый пакет - пиздец всему фрейму
                    fuBroken = true;
                    return false;
                }
                if ( prevFUFlag == H264_FUHeader::FU_FIRST) { //-- проебали середину, но впринципе, хуй с ними
                }
                //qDebug()<<"last";
                prevFUFlag = H264_FUHeader::FU_LAST;
                _frame.append( _curPacket.mid( getH264PayloadStart(), getH264PayloadLength() ) );
                toNewFrame(_frame);
                _frame.clear();
            } else{

            }
            break;
        }

        //-- пришли новые параметры картинки
        case SPS:
        case PPS:
        {
            qDebug()<<"SPS PPS";
            /*_frame.clear();
            prevNalType = _nalType;
            _frame.append(NAL_DELEMITER);
            _frame.append(_curPacket.mid( getH264PayloadStart(), getH264PayloadLength() ) );
             toNewFrame(_frame);
            _frame.clear();*/
            break;
        }

        default: {
            //qDebug()<<"Not implemented NAL type"<<QString::number(_nal & 0x1f);
            prevNalType = _nalType;
            break;
        }

    }

    return true;
}

}

