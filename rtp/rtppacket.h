#ifndef RTPPACKET_H
#define RTPPACKET_H

#include "interfaces/irtppacket.h"
#include <QDebug>
/**
* @brief Пакет RTP
*/
class RTPPacket: public virtual IRTPPacket
{
public:
  explicit RTPPacket(RTPPacket &&rtpPacket);
  explicit RTPPacket(const QByteArray &data);
  explicit RTPPacket(QByteArray &&data);
  explicit RTPPacket(size_t size);
  ~RTPPacket() {}

  static uint8_t getPayloadType(const QByteArray &data);

  uint8_t version() const override;
  void setVersion(uint8_t version) override;
  bool hasPadding() const override;
  void setHasPadding(bool hasPadding) override;
  bool hasExtension() const override;
  bool hasMarker() const override;
  void setHasMarker(bool hasMarker) override;
  uint8_t CSRCCount() const override;
  uint8_t payloadType() const override;
  void setPayloadType(uint8_t payloadType) override;
  uint16_t sequence() const override;
  void setSequence(uint16_t sequence) override;
  uint32_t timestamp() const override;
  void setTimestamp(uint32_t timestamp) override;
  uint32_t SSRC() const override;
  void setSSRC(uint32_t SSRC) override;
  QList<uint32_t> CSRC() const override;
  void setCSRC(const QList<uint32_t> &CSRC) override;

  uint16_t extensionHeaderId() const override;
  uint16_t extensionHeaderSize() const override;
  QByteArray extensionHeaderData() const override;
  void setExtensionHeader(uint16_t id, const QByteArray &data) override;
  uint8_t *payloadData() const override;
  uint32_t setPayloadData(uint8_t* data, uint32_t size) override;

  QByteArray const &data() const override;
  QByteArray &data() override;
  uint8_t *rawData() const override;
  void setData(const QByteArray &data) override;

  uint16_t payloadStart() const override;
  uint64_t payloadLength() const override;
  uint64_t size() const override;

  RTPPacket& operator=(const QByteArray &data);
  RTPPacket& operator=(const RTPPacket &packet);

protected:
  const int RTP_HEADER_SIZE =12; //-- Минимально возможный размер заголовка RTP пакета
  void setHasExtension(bool hasExtension) override;
  void setCSRCCount(uint8_t CSRCCount) override;
  QByteArray _data;

private:  


};

#endif // RTPPACKET_H
