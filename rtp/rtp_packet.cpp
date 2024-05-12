#include "rtp_packet.h"
#include <QDebug>

RTP_Packet::RTP_Packet(const QByteArray &data): _data(data)
{
  if ( _data.isEmpty() ) { _data.resize(RTP_HEADER_SIZE); } //-- Если не задан пакет, то сразу обеспечим минимальный размер для чтения основных полей заголовка
}


RTP_Packet::RTP_Packet(size_t size): _data(size, Qt::Uninitialized)
{
}

/**
* @brief Отдаём версию RTP пакета
* @return
*/
uint8_t RTP_Packet::version() const
{
  return ((_data[0]&0xC0)>>6); //-- Нас интересуют только первые 2 бита
}

/**
* @brief Задаём версию RTP пакета
* @param newVersion
*/
void RTP_Packet::setVersion(uint8_t version)
{
  _data[0] = (_data[0]&0x3F) | (version<<6); //-- Нас интересуют только первые 2 бита
}

/**
* @brief Отвечаем, имеет ли дополнительное выравнивание пакет (для шифрования и т.д.)
* @return
*/
bool RTP_Packet::hasPadding() const
{
  return (((_data[0]&0x20)>>5)==1);
}

/**
* @brief Задаём, имеет ли дополнительное выравнивание пакет
* @param newHasPadding
*/
void RTP_Packet::setHasPadding(bool hasPadding)
{
  _data[0] =(_data[0]&0xDF) | (hasPadding<<5); //-- Нас интересует только 3 бит
}

/**
* @brief Отвечаем, имеется ли дополнительный заголовок (после основного заголовка, перед данными)
* @return
*/
bool RTP_Packet::hasExtension() const
{
  return (((_data[0]&0x10)>>4)==1); //-- Нас интересует только 4 бит
}

/**
* @brief Задаём, имеется ли дополнительный заголовок (после основного заголовка, перед данными)
* @param newHasExtension
*/
void RTP_Packet::setHasExtension(bool hasExtension)
{
  _data[0] =(_data[0]&0xEF) | (hasExtension<<4); //-- Нас интересует только 4 бит
}

/**
* @brief Количество CSRC идентификаторов
* @return
*/
uint8_t RTP_Packet::CSRCCount() const
{
  return (uint16_t)((_data[0])&0x0F); //-- Нас интересуют только последние 4 бита
}

/**
* @brief Задаём новое количество CSRC идентификаторов
* @param newCSRCCount
*/
void RTP_Packet::setCSRCCount(uint8_t CSRCCount)
{
  _data[0] =(_data[0]&0xF0) | (CSRCCount); //-- Нас интересуют только последние 4 бита
}

/**
* @brief Отвечаем, имеют ли данные какое-то особое значение (вне спецификации формата)
* @return
*/
bool RTP_Packet::hasMarker() const
{
  return (((_data[1]&0x80)>>7)==1); //-- Интересует только первый бит
}

/**
* @brief Задаём, имеют ли данные какое-то особое значение (вне спецификации формата)
* @param newHasMarker
*/
void RTP_Packet::setHasMarker(bool hasMarker)
{
  _data[1] = (_data[1]&0x7F) | (hasMarker<<7); //-- Интересует только первый бит
}

/**
* @brief Тип данных в пакете
* @return
*/
uint8_t RTP_Packet::payloadType() const
{
  return _data[1]&0x7F; //-- Нас интересует только последние 7 бит
}

/**
* @brief Задаём новый тип данных в пакете
* @param newPayloadType
*/
void RTP_Packet::setPayloadType(uint8_t payloadType)
{
  _data[1] =(_data[1]&0x80) | payloadType; //-- Нас интересует только последние 7 бит
}

/**
* @brief Порядковый номер пакета
* @return
*/
uint16_t RTP_Packet::sequence() const
{
  return ( //-- Собираем 2 байта в uint16_t
    ((_data[2]<<8) & 0xFF00) |
    ((_data[3]<<0) & 0xFF)
  );
}

/**
* @brief Задаём новый порядковый номер пакета
* @param sequence
*/
void RTP_Packet::setSequence(uint16_t sequence)
{
  _data[2] =(sequence>>8)&0xFF;
  _data[3] =(sequence>>0)&0xFF;
}

/**
* @brief Отдаём метку времени
* @return
*/
uint32_t RTP_Packet::timestamp() const
{
  return ( //-- Собираем 4 байта в int
    ((_data[4]<<24) & 0xFF000000L) |
    ((_data[5]<<16) & 0xFF0000L) |
    ((_data[6]<<8) & 0xFF00L) |
    ((_data[7]<<0) & 0xFFL)
  );
}

/**
* @brief Задаём новую метку времени
* @param newTimestamp
*/
void RTP_Packet::setTimestamp(uint32_t timestamp)
{
  _data[4] =(timestamp>>24)&0xFF;
  _data[5] =(timestamp>>16)&0xFF;
  _data[6] =(timestamp>>8)&0xFF;
  _data[7] =(timestamp>>0)&0xFF;
}

/**
* @brief Отдаём айдишник источника синхронизации
* @return
*/
uint32_t RTP_Packet::SSRC() const
{
  return ( //-- Собираем 4 байта в int
    ((_data[8]<<24) & 0xFF000000L) |
    ((_data[9]<<16) & 0xFF0000L) |
    ((_data[10]<<8) & 0xFF00L) |
    ((_data[11]<<0) & 0xFFL)
  );
}

/**
* @brief Задаём новый айдишник источника синхронизации
* @param newSSRC
*/
void RTP_Packet::setSSRC(uint32_t SSRC)
{
  _data[8] =(SSRC>>24)&0xFF;
  _data[9] =(SSRC>>16)&0xFF;
  _data[10] =(SSRC>>8)&0xFF;
  _data[11] =(SSRC>>0)&0xFF;
}

/**
* @brief Отдаём список CSRC идентификаторов
* @return
*/
QList<uint32_t> RTP_Packet::CSRC() const
{
  QList<uint32_t>listCSRC;
  for(uint8_t i=0; i<CSRCCount(); ++i) {
    listCSRC.append(
      ((_data[i*4+12+0]<<24) & 0xFF000000L) |
      ((_data[i*4+12+1]<<16) & 0xFF0000L) |
      ((_data[i*4+12+2]<<8) & 0xFF00L) |
      ((_data[i*4+12+3]<<0) & 0xFFL)
    );
  }
  return listCSRC;
}

/**
* @brief Задаём список CSRC идентификаторов
* @param newCSRC
*/
void RTP_Packet::setCSRC(const QList<uint32_t> &CSRC)
{
  for (uint8_t i=0; i<CSRC.count(); ++i) {
    _data[i*4+12+0] =(CSRC[i]>>24)&0xFF;
    _data[i*4+12+1] =(CSRC[i]>>16)&0xFF;
    _data[i*4+12+2] =(CSRC[i]>>8)&0xFF;
    _data[i*4+12+3] =(CSRC[i]>>0)&0xFF;
  }
  setCSRCCount(CSRC.count());
}

/**
* @brief Отдаём айдишник дополнительного заголовка
* @return
*/
uint16_t RTP_Packet::extensionHeaderId() const
{
  if ( !hasExtension() ) { return 0; }
  uint8_t csrcc = CSRCCount();
  return ( //-- Собираем 2 байта в uint16_t
    ((_data[12+4*csrcc+0]<<8) & 0xFF00) |
    ((_data[12+4*csrcc+1]<<0) & 0xFF)
  );
}

/**
* @brief Отдаём размер дополнительного заголовка
* @return
*/
uint16_t RTP_Packet::extensionHeaderSize() const
{
  if ( !hasExtension() ) { return 0; }
  uint8_t csrcc = CSRCCount();
  return ( //-- Собираем 2 байта в uint16_t
    ((_data[12+4*csrcc+2]<<8) & 0xFF00) |
    ((_data[12+4*csrcc+3]<<0) & 0xFF)
  );
}

/**
* @brief Отдаём дополнительный заголовок
* @return
*/
QByteArray RTP_Packet::extensionHeaderData() const
{
  if ( !hasExtension() ) { return QByteArray(); }
  return _data.mid(16+4*CSRCCount(), extensionHeaderSize());
}

/**
* @brief задаём новый дополнительный заголовок
* @param newExtensionHeader
*/
void RTP_Packet::setExtensionHeader(uint16_t id, const QByteArray &data)
{
  if ( data.size()==0 ) { setHasExtension(false); return; }

  uint8_t csrcc = CSRCCount();
  _data[12+4*csrcc+0] =(id>>8)&0xFF;
  _data[12+4*csrcc+1] =(id>>0)&0xFF;
  _data[12+4*csrcc+2] =(data.size()>>8)&0xFF;
  _data[12+4*csrcc+3] =(data.size()>>0)&0xFF;
  _data.replace(16+4*csrcc, data.length(), data);

  setHasExtension(true);
}

/**
* @brief Отдаём указатель на данные полезной нагрузки
* @return
*/
uint8_t *RTP_Packet::payloadData() const
{
  return (uint8_t*)(_data.constData()+RTP_Packet::payloadStart());
}

/**
* @brief Задаём новые данные полезной нагрузки
* @param data
* @param size
*/
uint32_t RTP_Packet::setPayloadData(uint8_t *data, uint32_t size)
{
  _data.replace(RTP_Packet::payloadStart(), size, (const char *)(data), size);
  return size;
}

/**
* @brief Отдаём данные пакета целиком
* @return
*/
const QByteArray &RTP_Packet::data() const
{
  return _data;
}

/**
* @brief Отдаём данные пакета целиком
* @return
*/
QByteArray &RTP_Packet::data()
{
  return _data;
}

/**
* @brief Отдаём указатель на сырые данные
* @return
*/
uint8_t *RTP_Packet::rawData() const
{
  return (uint8_t *)_data.data();
}

/**
* @brief Задаём новые данные пакета целиком
* @param data
*/
void RTP_Packet::setData(const QByteArray &data)
{
  _data =data;
  if ( _data.isEmpty() ) { _data.resize(RTP_HEADER_SIZE); } //-- Если не задан пакет, то сразу обеспечим минимальный размер для чтения основных полей заголовка
}

/**
* @brief Размер пакета целиком
* @return
*/
uint64_t RTP_Packet::size() const
{
  return _data.size();
}

/**
* @brief RTP_Packet::operator =
* @param data
* @return
*/
RTP_Packet &RTP_Packet::operator=(const QByteArray &data)
{
  setData(data);
  return *this;
}

/**
* @brief RTP_Packet::operator =
* @param packet
* @return
*/
RTP_Packet &RTP_Packet::operator=(const RTP_Packet &packet)
{
  setData(packet._data);
  return *this;
}

/**
* @brief Отвечаем по какому смещению начинаются данные полезной нагрузки
* @return
*/
uint16_t RTP_Packet::payloadStart() const
{
  return (
    RTP_HEADER_SIZE
    +CSRCCount()*4
    +(hasExtension()? extensionHeaderSize()+4 : 0)
  );
}

/**
* @brief Отвечаем о размере данных полезной нагрузки
* @return
*/
uint64_t RTP_Packet::payloadLength() const
{
  return _data.length()-payloadStart();
}
