#ifndef RTPPACKETH265UNITFU_H
#define RTPPACKETH265UNITFU_H

#include "rtppacketh265.h"
#include "interfaces/irtppacketh265unitfu.h"

/**
* @brief NAL пакет с содержимым FU  (один NAL пакет фрагментированный на несколько)
*/
class RTPPacketH265UnitFU: public RTPPacketH265, public virtual IRTPPacketH265UnitFU
{
public:
  explicit RTPPacketH265UnitFU(const QByteArray &data);
  explicit RTPPacketH265UnitFU(QByteArray &&data);

  uint8_t fuIndicator() const override;
  bool setFUIndicator(uint8_t FUIndicator) override;
  uint8_t fuHeader() const override;
  bool setFUHeader(uint8_t FUHeader) override;
  FU_PARTS fuPart() const override;
  bool setFUPart(FU_PARTS FUPart) override;
  uint8_t fuType() const override;
  bool setFUType(uint8_t FUType) override;

  uint16_t payloadStart() const override;

protected:

};

#endif // RTPPACKETH265UNITFU_H
