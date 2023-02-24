#include "rtp_packet_h264_unit_fu.h"

RTP_Packet_H264_Unit_FU::RTP_Packet_H264_Unit_FU(const QByteArray &data): RTP_Packet_H264(data)
{

}

/**
* @brief Отдаём индикатор FU юнита целиком
* То же что и у NAL
* @return
*/
uint8_t RTP_Packet_H264_Unit_FU::fuIndicator() const
{
  return nalHeader();
}

/**
* @brief Задаём индикатор FU юнита целиком
* @param newFUIndicator
* @return
*/
bool RTP_Packet_H264_Unit_FU::setFUIndicator(uint8_t FUIndicator)
{
  return setNALHeader(FUIndicator);
}

/**
* @brief Отдаём заголовок FU сожержимого целиком
* @return
*/
uint8_t RTP_Packet_H264_Unit_FU::fuHeader() const
{
  return _data[RTP_Packet::payloadStart()+1];
}

/**
* @brief Задаём заголовок FU сожержимого целиком
* @param newFUHeader
* @return
*/
bool RTP_Packet_H264_Unit_FU::setFUHeader(uint8_t FUHeader)
{
  _data[RTP_Packet::payloadStart()+1] =FUHeader&0xDF; //-- Бит 3 зарезеривирован и должен быть 0;
  return true;
}

/**
* @brief Отдаём какая часть FU пакета
* @return
*/
RTP_Packet_H264_Unit_FU::FU_PARTS RTP_Packet_H264_Unit_FU::fuPart() const
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
bool RTP_Packet_H264_Unit_FU::setFUPart(FU_PARTS FUPart)
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
* @brief RTP_Packet_H264_Unit_FU::fuType
* @return
*/
uint8_t RTP_Packet_H264_Unit_FU::fuType() const
{
  return fuHeader()&0x1F;
}

bool RTP_Packet_H264_Unit_FU::setFUType(uint8_t FUType)
{
  uint8_t fuh =fuHeader()&0xE0; //-- Только последние 5 бит нас интересуют
  return setFUHeader(fuh | FUType);
}

/**
* @brief Перегружаем смещение полезного содержимого
* @return
*/
uint16_t RTP_Packet_H264_Unit_FU::payloadStart() const
{
  return RTP_Packet::payloadStart()+2; //-- У нас 1 байт индикатор FU + 1 байт заголовок FU
}
