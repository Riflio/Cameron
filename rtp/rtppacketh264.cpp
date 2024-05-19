#include "rtppacketh264.h"

RTPPacketH264::RTPPacketH264(RTPPacket &&rtpPacket): RTPPacket(std::move(rtpPacket))
{
}

RTPPacketH264::RTPPacketH264(const QByteArray &data): RTPPacket(data)
{
}

RTPPacketH264::RTPPacketH264(QByteArray &&data): RTPPacket(std::move(data))
{
}

/**
* @brief Отдаём заголовок пакета
* @return
*/
uint8_t RTPPacketH264::nalHeader() const
{
  return _data[RTPPacket::payloadStart()];
}

bool RTPPacketH264::setNALHeader(uint8_t nalHeader)
{
  _data[RTPPacket::payloadStart()] =nalHeader&0x7F; //-- Бит 1 всегда должен быть 0
  return true;
}

/**
* @brief Отдаём тип NAL юнита
* @return
*/
RTPPacketH264::NAL_TYPES RTPPacketH264::nalType() const
{
  return (NAL_TYPES)(_data[RTPPacket::payloadStart()]&0x1F); //-- Последние 5 бит
}

bool RTPPacketH264::setNALType(NAL_TYPES nalType)
{
  uint8_t nalh =RTPPacketH264::nalHeader()&0xE0; //-- Только последние 5 бит нас интересуют
  return setNALHeader(nalh | nalType);
}

/**
* @brief Отдаём ссылку на другие NAL
* @return
*/
uint8_t RTPPacketH264::nalRefIDC() const
{
  return (RTPPacketH264::nalHeader()&0x60)<<1; //-- Только биты 2 и 3 нас интересуют
}

bool RTPPacketH264::setNALRefIDC(uint8_t nalRefIDC)
{
  uint8_t nalh =RTPPacketH264::nalHeader()&0x1F; //-- Только биты 2 и 3 нас интересуют
  return RTPPacketH264::setNALHeader(nalh | (nalRefIDC&0x3)<<5);
}

/**
* @brief Отвечаем FU ли это юнит (один NAL пакет фрагментированный на несколько)
* @return
*/
bool RTPPacketH264::isFUUnit() const
{
  RTPPacketH264::NAL_TYPES t =RTPPacketH264::nalType();
  return ( t==FU_A || t==FU_B );
}

/**
* @brief RTPPacketH264::payloadData
* @return
*/
uint8_t *RTPPacketH264::payloadData() const
{
  return (uint8_t*)(_data.constData()+RTPPacketH264::payloadStart());
}

/**
* @brief RTPPacketH264::payloadStart
* @return
*/
uint16_t RTPPacketH264::payloadStart() const
{
  return RTPPacket::payloadStart()+1; //-- У нас 1 байт наш nalHeader()
}

/**
* @brief RTPPacketH264::payloadLength
* @return
*/
uint64_t RTPPacketH264::payloadLength() const
{
  return RTPPacket::payloadLength()-1;
}

