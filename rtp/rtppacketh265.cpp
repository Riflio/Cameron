#include "rtppacketh265.h"

RTPPacketH265::RTPPacketH265(const QByteArray &data): RTPPacket(data)
{
}

RTPPacketH265::RTPPacketH265(QByteArray &&data): RTPPacket(std::move(data))
{
}

/**
* @brief Отдаём заголовок пакета
* @return
*/
uint16_t RTPPacketH265::nalHeader() const
{
  return (_data[RTPPacket::payloadStart()] | _data[RTPPacket::payloadStart()+1]<<8);
}

bool RTPPacketH265::setNALHeader(uint16_t nalHeader)
{
  _data[RTPPacket::payloadStart()] =(nalHeader&0xff00)>>8;
  _data[RTPPacket::payloadStart()+1] =(nalHeader&0x00ff);
  return true;
}

/**
* @brief Отдаём тип NAL юнита
* @return
*/
RTPPacketH265::NAL_TYPES RTPPacketH265::nalType() const
{
  return (NAL_TYPES)((_data[RTPPacket::payloadStart()+2]>>1)&0x3F);
}

bool RTPPacketH265::setNALType(NAL_TYPES nalType)
{
  uint8_t nalh =nalHeader()&0xC0;
  return setNALHeader(nalh | nalType);
}

/**
* @brief Отвечаем FU ли это юнит (один NAL пакет фрагментированный на несколько)
* @return
*/
bool RTPPacketH265::isFUUnit() const
{
  return (nalType()==NAL_TYPES::FU);
}

/**
* @brief RTP_Packet_H264::payloadStart
* @return
*/
uint16_t RTPPacketH265::payloadStart() const
{
  return RTPPacket::payloadStart()+2; //-- У нас 2 байта наш nalHeader()
}


