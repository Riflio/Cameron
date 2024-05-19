#include "rtppacketh265unitfu.h"

RTPPacketH265UnitFU::RTPPacketH265UnitFU(const QByteArray &data): RTPPacketH265(data)
{
}

RTPPacketH265UnitFU::RTPPacketH265UnitFU(QByteArray &&data): RTPPacketH265(std::move(data))
{
}

/**
* @brief Отдаём индикатор FU юнита целиком
* То же что и у NAL
* @return
*/
uint8_t RTPPacketH265UnitFU::fuIndicator() const
{
  return nalHeader();
}

/**
* @brief Задаём индикатор FU юнита целиком
* @param newFUIndicator
* @return
*/
bool RTPPacketH265UnitFU::setFUIndicator(uint8_t FUIndicator)
{
  return setNALHeader(FUIndicator);
}

/**
* @brief Отдаём заголовок FU сожержимого целиком
* @return
*/
uint8_t RTPPacketH265UnitFU::fuHeader() const
{
  return _data[RTPPacketH265::payloadStart()];
}

/**
* @brief Задаём заголовок FU сожержимого целиком
* @param newFUHeader
* @return
*/
bool RTPPacketH265UnitFU::setFUHeader(uint8_t FUHeader)
{
  _data[RTPPacketH265::payloadStart()] =FUHeader;
  return true;
}

/**
* @brief Отдаём какая часть FU пакета
* @return
*/
RTPPacketH265UnitFU::FU_PARTS RTPPacketH265UnitFU::fuPart() const
{
  if ( ((fuHeader()&0x80)>>7)==1 ) { return FU_START; }
  if ( ((fuHeader()&0x40)>>6)==1 ) { return FU_END; }
  return FU_MIDDLE;
}

/**
* @brief Задаём часть FU пакета
* @param fuPart
* @return
*/
bool RTPPacketH265UnitFU::setFUPart(FU_PARTS FUPart)
{
  uint8_t fuh =fuHeader()&0x3F; //-- Только первые 2 бита нас интересуют

  switch (FUPart) {
    case FU_NOT: { return false; }
    case FU_START: { fuh |=0x80;  break; }
    case FU_END: { fuh |=0x40; break; }
    case FU_MIDDLE: { break; }
  }

  return setFUHeader(fuh);
}

/**
* @brief RTP_Packet_H265_Unit_FU::fuType
* @return
*/
uint8_t RTPPacketH265UnitFU::fuType() const
{
  return fuHeader()&0x3F;
}

/**
* @brief RTP_Packet_H265_Unit_FU::setFUType
* @param FUType
* @return
*/
bool RTPPacketH265UnitFU::setFUType(uint8_t FUType)
{
  uint8_t fuh =fuHeader()&0xC0; //-- Только последние 6 бит нас интересуют
  return setFUHeader(fuh | FUType);
}

/**
* @brief Перегружаем смещение полезного содержимого
* @return
*/
uint16_t RTPPacketH265UnitFU::payloadStart() const
{
  return RTPPacketH265::payloadStart()+1;
}
