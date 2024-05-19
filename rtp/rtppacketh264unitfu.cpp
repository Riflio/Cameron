#include "rtppacketh264unitfu.h"

RTPPacketH264UnitFU::RTPPacketH264UnitFU(const QByteArray &data): RTPPacketH264(data)
{
}

RTPPacketH264UnitFU::RTPPacketH264UnitFU(QByteArray &&data): RTPPacketH264(std::move(data))
{
}

RTPPacketH264UnitFU::RTPPacketH264UnitFU(RTPPacketH264 &&rtpPacketH264): RTPPacketH264(std::move(rtpPacketH264))
{
}

/**
* @brief Заголовок
* @return
*/
uint8_t RTPPacketH264UnitFU::nalHeader() const
{
  //-- Когда фрагментированный пакет нужно восстановить заголовок - первые 3 бита из FU indicator и 5 последних FU header
  return ((RTPPacketH264UnitFU::fuIndicator()&0xE0)^(RTPPacketH264UnitFU::fuHeader()&0x1f));
}

/**
* @brief Отдаём индикатор FU юнита целиком
* То же что и у NAL
* @return
*/
uint8_t RTPPacketH264UnitFU::fuIndicator() const
{
  return RTPPacketH264::nalHeader();
}

/**
* @brief Задаём индикатор FU юнита целиком
* @param newFUIndicator
* @return
*/
bool RTPPacketH264UnitFU::setFUIndicator(uint8_t FUIndicator)
{
  return RTPPacketH264::setNALHeader(FUIndicator);
}

/**
* @brief Отдаём заголовок FU сожержимого целиком
* @return
*/
uint8_t RTPPacketH264UnitFU::fuHeader() const
{
  return _data[RTPPacketH264::payloadStart()];
}

/**
* @brief Задаём заголовок FU сожержимого целиком
* @param newFUHeader
* @return
*/
bool RTPPacketH264UnitFU::setFUHeader(uint8_t FUHeader)
{
  _data[RTPPacketH264::payloadStart()] =FUHeader&0xDF; //-- Бит 3 зарезеривирован и должен быть 0;
  return true;
}

/**
* @brief Отдаём какая часть FU пакета
* @return
*/
RTPPacketH264UnitFU::FU_PARTS RTPPacketH264UnitFU::fuPart() const
{
  if ( ((RTPPacketH264UnitFU::fuHeader()&0x80)>>7)==1 ) { return FU_START; }
  if ( ((RTPPacketH264UnitFU::fuHeader()&0x40)>>6)==1 ) { return FU_END; }
  return FU_MIDDLE;
}

/**
* @brief Задаём часть FU пакета
* @param fuPart
* @return
*/
bool RTPPacketH264UnitFU::setFUPart(FU_PARTS FUPart)
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
* @brief RTPPacketH264UnitFU::fuType
* @return
*/
uint8_t RTPPacketH264UnitFU::fuType() const
{
  return fuHeader()&0x1F;
}

bool RTPPacketH264UnitFU::setFUType(uint8_t FUType)
{
  uint8_t fuh =fuHeader()&0xE0; //-- Только последние 5 бит нас интересуют
  return setFUHeader(fuh | FUType);
}

/**
* @brief RTPPacketH264UnitFU::payloadData
* @return
*/
uint8_t *RTPPacketH264UnitFU::payloadData() const
{
  return (uint8_t*)(_data.constData()+RTPPacketH264UnitFU::payloadStart());
}

/**
* @brief Перегружаем смещение полезного содержимого
* @return
*/
uint16_t RTPPacketH264UnitFU::payloadStart() const
{
  return RTPPacketH264::payloadStart()+1; //-- 1 байт заголовок FU
}

/**
* @brief RTPPacketH264UnitFU::payloadLength
* @return
*/
uint64_t RTPPacketH264UnitFU::payloadLength() const
{
  return RTPPacketH264::payloadLength()-1;
}
