#ifndef RTPPACKETH264_H
#define RTPPACKETH264_H

#include "rtppacket.h"
#include "interfaces/irtppacketh264.h"
/**
* @brief RTP пакет с данными видео формата H264
* Базовый NAL unit
* @ref RFC6184
*/
class RTPPacketH264: public RTPPacket, public virtual IRTPPacketH264
{
public:
  explicit RTPPacketH264(RTPPacket &&rtpPacket);
  explicit RTPPacketH264(const QByteArray &data);
  explicit RTPPacketH264(QByteArray &&data);  

  uint8_t nalHeader() const override;
  bool setNALHeader(uint8_t nalHeader) override;
  NAL_TYPES nalType() const override;
  bool setNALType(NAL_TYPES nalType) override;
  uint8_t nalRefIDC() const override;
  bool setNALRefIDC(uint8_t nalRefIDC) override;

  bool isFUUnit() const override;

  uint8_t *payloadData() const override;
  uint16_t payloadStart() const override;
  uint64_t payloadLength() const override;

private:


};

#endif // RTPPACKETH264_H
