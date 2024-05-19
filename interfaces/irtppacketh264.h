#ifndef IRTPPACKETH264_H
#define IRTPPACKETH264_H

#include "interfaces/irtppacket.h"

class IRTPPacketH264: public virtual IRTPPacket
{
public:
  virtual ~IRTPPacketH264() {}

   ///@brief Тип пакета
  enum NAL_TYPES {
    FU_A =28, ///< Фрагментированный
    FU_B =29, ///< Фрагментированный
    SPS =7, ///< Метаданные потоке кадров, изображении и т.д.
    PPS =8, ///< Метаданные потоке кадров, изображении и т.д.
    IDR =5, ///< Пакет со всем необходимым для начала декодирования (ключевой кадр?) без ссылок на другие кадры
    NON_IDR =1, ///< Пакет с сылками на другие кадры
    SEI =6, ///< Supplemental Enhancement Information (SEI), хз короче, пусть будет
    MAX =40 ///< Максимальный
  };

  const QByteArray NAL_DELEMITER =QByteArrayLiteral("\x00\x00\x00\x01"); //-- Разделитель NAL блоков 00 00 00 01

  virtual uint8_t nalHeader() const =0;
  virtual bool setNALHeader(uint8_t nalHeader) =0;
  virtual  NAL_TYPES nalType() const =0;
  virtual  bool setNALType(NAL_TYPES nalType) =0;
  virtual  uint8_t nalRefIDC() const =0;
  virtual  bool setNALRefIDC(uint8_t nalRefIDC) =0;
  virtual  bool isFUUnit() const =0;

};

#endif // IRTPPACKETH264_H
