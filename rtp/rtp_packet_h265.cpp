#include "rtp_packet_h265.h"

RTP_Packet_H265::RTP_Packet_H265(const QByteArray &data): RTP_Packet(data)
{

}

/**
* @brief Отдаём заголовок пакета
* @return
*/
uint16_t RTP_Packet_H265::nalHeader() const
{
  return (_data[RTP_Packet::payloadStart()] | _data[RTP_Packet::payloadStart()+1]<<8);
}

bool RTP_Packet_H265::setNALHeader(uint16_t nalHeader)
{
  _data[RTP_Packet::payloadStart()] =(nalHeader&0xff00)>>8;
  _data[RTP_Packet::payloadStart()+1] =(nalHeader&0x00ff);
  return true;
}

/**
* @brief Отдаём тип NAL юнита
* @return
*/
RTP_Packet_H265::NAL_TYPES RTP_Packet_H265::nalType() const
{
  return (NAL_TYPES)((_data[RTP_Packet::payloadStart()+2]>>1)&0x3F);
}

bool RTP_Packet_H265::setNALType(NAL_TYPES nalType)
{
  uint8_t nalh =nalHeader()&0xC0;
  return setNALHeader(nalh | nalType);
}

/**
* @brief Отвечаем FU ли это юнит (один NAL пакет фрагментированный на несколько)
* @return
*/
bool RTP_Packet_H265::isFUUnit() const
{
  return (nalType()==NAL_TYPES::FU);
}

/**
* @brief RTP_Packet_H264::payloadStart
* @return
*/
uint16_t RTP_Packet_H265::payloadStart() const
{
  return RTP_Packet::payloadStart()+2; //-- У нас 2 байта наш nalHeader()
}


