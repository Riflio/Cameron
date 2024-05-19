#ifndef IRTPPACKETH264UNITFU_H
#define IRTPPACKETH264UNITFU_H

#include "interfaces/irtppacketh264.h"

class IRTPPacketH264UnitFU: public virtual IRTPPacketH264
{
public:
  virtual ~IRTPPacketH264UnitFU() {}

  //-- Fragment Unit part
  enum FU_PARTS {
    FU_NOT, //-- Not a FU unit
    FU_START,
    FU_MIDDLE,
    FU_END
  };

  virtual uint8_t fuIndicator() const =0;
  virtual bool setFUIndicator(uint8_t FUIndicator) =0;
  virtual uint8_t fuHeader() const =0;
  virtual bool setFUHeader(uint8_t FUHeader) =0;
  virtual FU_PARTS fuPart() const =0;
  virtual bool setFUPart(FU_PARTS FUPart) =0;
  virtual uint8_t fuType() const =0;
  virtual bool setFUType(uint8_t FUType) =0;

};

#endif // IRTPPACKETH264UNITFU_H
