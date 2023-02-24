#include "rtp_packet_h264.h"

RTP_Packet_H264::RTP_Packet_H264(const QByteArray &data): RTP_Packet(data)
{

}

/**
* @brief Отдаём заголовок пакета
* @return
*/
uint8_t RTP_Packet_H264::nalHeader() const
{
  return _data[RTP_Packet::payloadStart()];
}

bool RTP_Packet_H264::setNALHeader(uint8_t nalHeader)
{
  _data[RTP_Packet::payloadStart()] =nalHeader&0x7F; //-- Бит 1 всегда должен быть 0
  return true;
}

/**
* @brief Отдаём тип NAL юнита
* @return
*/
RTP_Packet_H264::NAL_TYPES RTP_Packet_H264::nalType() const
{
  return (NAL_TYPES)(_data[RTP_Packet::payloadStart()]&0x1F);
}

bool RTP_Packet_H264::setNALType(NAL_TYPES nalType)
{
  uint8_t nalh =nalHeader()&0xE0; //-- Только последние 5 бит нас интересуют
  return setNALHeader(nalh | nalType);
}

/**
* @brief Отдаём ссылку на другие NAL
* @return
*/
uint8_t RTP_Packet_H264::nalRefIDC() const
{
  return (nalHeader()&0x60)<<1; //-- Только биты 2 и 3 нас интересуют
}

bool RTP_Packet_H264::setNALRefIDC(uint8_t nalRefIDC)
{
  uint8_t nalh =nalHeader()&0x1F; //-- Только биты 2 и 3 нас интересуют
  return setNALHeader(nalh | (nalRefIDC&0x3)<<5);
}

/**
* @brief Отвечаем FU ли это юнит (один NAL пакет фрагментированный на несколько)
* @return
*/
bool RTP_Packet_H264::isFUUnit() const
{
  return ( (nalType()==FU_A || nalType()==FU_B) );
}

/**
* @brief RTP_Packet_H264::payloadStart
* @return
*/
uint16_t RTP_Packet_H264::payloadStart() const
{
  return RTP_Packet::payloadStart()+1; //-- У нас 1 байт наш nalHeader()
}


