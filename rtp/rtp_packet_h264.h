#ifndef RTP_PACKET_H264_H
#define RTP_PACKET_H264_H

#include "rtp_packet.h"

/**
* @brief RTP пакет с данными видео формата H264
* Базовый NAL unit
*/
class RTP_Packet_H264 : public RTP_Packet
{
public:
  explicit RTP_Packet_H264(const QByteArray &data =QByteArray());

  //-- Тип пакета
  enum NAL_TYPES {
    FU_A = 28, //-- Фрагментированный
    FU_B = 29, //-- Фрагментированный
    SPS = 7, //-- Метаданные потоке кадров, изображении и т.д.
    PPS = 8, //-- Метаданные потоке кадров, изображении и т.д.
    IDR = 5, //-- Пакет со всем необходимым для начала декодирования (ключевой кадр?) без ссылок на другие кадры
    NON_IDR = 1, //-- Пакет с сылками на другие кадры
    SEI = 6 //-- Supplemental Enhancement Information (SEI), хз короче, пусть будет
  };

  const QByteArray NAL_DELEMITER = QByteArray::fromRawData("\x00\x00\x00\x01", 4); //-- Разделитель NAL блоков 00 00 00 01

  uint8_t nalHeader() const;
  bool setNALHeader(uint8_t nalHeader);
  NAL_TYPES nalType() const;
  bool setNALType(NAL_TYPES nalType);
  uint8_t nalRefIDC() const;
  bool setNALRefIDC(uint8_t nalRefIDC);

  bool isFUUnit() const;

  uint16_t payloadStart() const override;

private:


};

#endif // RTP_PACKET_H264_H
