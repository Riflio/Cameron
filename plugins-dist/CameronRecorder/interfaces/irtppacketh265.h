#ifndef IRTPPACKETH265_H
#define IRTPPACKETH265_H

#include "interfaces/irtppacket.h"

class IRTPPacketH265: public virtual IRTPPacket
{
public:
  virtual ~IRTPPacketH265() {}

  /// @brief Тип пакета
  enum NAL_TYPES {
    FU =49, ///< Fragmented Unit
    VPS =32,
    SPS =33, ///< Sequence parameter set
    PPS =34, ///< Picture parameter set
    SEI =39, ///< Supplemental enhancement information
    PACI =50
    //-- Other =Single NAL Unit Packets
  };

  const QByteArray NAL_DELEMITER =QByteArrayLiteral("\x00\x00\x00\x01"); //-- Разделитель NAL блоков 00 00 00 01

  virtual uint16_t nalHeader() const =0;
  virtual bool setNALHeader(uint16_t nalHeader) =0;
  virtual NAL_TYPES nalType() const =0;
  virtual bool setNALType(NAL_TYPES nalType) =0;

  virtual bool isFUUnit() const =0;

};

#endif // IRTPPACKETH265_H
