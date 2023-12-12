#ifndef RTP_PACKET_H265_H
#define RTP_PACKET_H265_H

#include "rtp_packet.h"

/**
* @brief RTP пакет с данными видео формата H265
* Базовый NAL unit
* @ref RFC7798
*/
class RTP_Packet_H265: public RTP_Packet
{
public:
  explicit RTP_Packet_H265(const QByteArray &data =QByteArray());

  //-- Тип пакета
  enum NAL_TYPES {
    FU =49, //-- Fragmented Unit
    VPS =32,
    SPS =33, //-- Sequence parameter set
    PPS =34, //-- Picture parameter set
    SEI =39, //-- Supplemental enhancement information
    PACI =50
    //-- Other =Single NAL Unit Packets
  };

  uint16_t nalHeader() const;
  bool setNALHeader(uint16_t nalHeader);
  NAL_TYPES nalType() const;
  bool setNALType(NAL_TYPES nalType);

  bool isFUUnit() const;

  uint16_t payloadStart() const override;

private:


};

#endif // RTP_PACKET_H265_H
