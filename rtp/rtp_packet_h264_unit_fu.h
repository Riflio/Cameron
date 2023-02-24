#ifndef RTP_PACKET_H264_UNIT_FU_H
#define RTP_PACKET_H264_UNIT_FU_H

#include "rtp_packet_h264.h"

/**
* @brief NAL пакет с содержимым FU  (один NAL пакет фрагментированный на несколько)
*/
class RTP_Packet_H264_Unit_FU : public RTP_Packet_H264
{
public:
  RTP_Packet_H264_Unit_FU(const QByteArray &data =QByteArray());

  //-- Fragment Unit part
  enum FU_PARTS {
    FU_NOT, //-- Not a FU unit
    FU_START,
    FU_MIDDLE,
    FU_END
  };

  uint8_t fuIndicator() const;
  bool setFUIndicator(uint8_t FUIndicator);
  uint8_t fuHeader() const;
  bool setFUHeader(uint8_t FUHeader);
  FU_PARTS fuPart() const;
  bool setFUPart(FU_PARTS FUPart);
  uint8_t fuType() const;
  bool setFUType(uint8_t FUType);

  uint16_t payloadStart() const override;

protected:

};

#endif // RTP_PACKET_H264_UNIT_FU_H
