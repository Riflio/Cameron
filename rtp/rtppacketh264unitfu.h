#ifndef RTPPACKETH264UNITFU_H
#define RTPPACKETH264UNITFU_H

#include "rtppacketh264.h"
#include "interfaces/irtppacketh264unitfu.h"

/**
* @brief NAL пакет с содержимым FU  (один NAL пакет фрагментированный на несколько)
*/
class RTPPacketH264UnitFU: public RTPPacketH264, public virtual IRTPPacketH264UnitFU
{
public:
  explicit RTPPacketH264UnitFU(const QByteArray &data);
  explicit RTPPacketH264UnitFU(QByteArray &&data);
  explicit RTPPacketH264UnitFU(RTPPacketH264 &&rtpPacketH264);

  uint8_t nalHeader() const override;
  uint8_t fuIndicator() const override;
  bool setFUIndicator(uint8_t FUIndicator) override;
  uint8_t fuHeader() const override;
  bool setFUHeader(uint8_t FUHeader) override;
  FU_PARTS fuPart() const override;
  bool setFUPart(FU_PARTS FUPart) override;
  uint8_t fuType() const override;
  bool setFUType(uint8_t FUType) override;

  uint8_t *payloadData() const override;
  uint16_t payloadStart() const override;
  uint64_t payloadLength() const override;

protected:

};

#endif // RTPPACKETH264UNITFU_H
