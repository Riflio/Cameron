#ifndef RTPPACKETH265_H
#define RTPPACKETH265_H

#include "rtppacket.h"
#include "interfaces/irtppacketh265.h"

/**
* @brief RTP пакет с данными видео формата H265
* Базовый NAL unit
* @ref RFC7798
*/
class RTPPacketH265: public RTPPacket, public virtual IRTPPacketH265
{
public:
  explicit RTPPacketH265(const QByteArray &data);
  explicit RTPPacketH265(QByteArray &&data);

  uint16_t nalHeader() const override;
  bool setNALHeader(uint16_t nalHeader) override;
  NAL_TYPES nalType() const override;
  bool setNALType(NAL_TYPES nalType) override;
  bool isFUUnit() const override;
  uint16_t payloadStart() const override;

private:


};

#endif // RTPPACKETH265_H
