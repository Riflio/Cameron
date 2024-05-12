#ifndef RTP_PACKET_H
#define RTP_PACKET_H

#include "interfaces/irtp_packet.h"
#include <QDebug>
/**
* @brief Пакет RTP
*/
class RTP_Packet: public IRTP_Packet
{
public:
  RTP_Packet(const QByteArray &data =QByteArray());
  RTP_Packet(size_t size);
  ~RTP_Packet() {}

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

  RTP_Packet& operator=(const QByteArray &data);
  RTP_Packet& operator=(const RTP_Packet &packet);

protected:
  const int RTP_HEADER_SIZE =12; //-- Минимально возможный размер заголовка RTP пакета
  void setHasExtension(bool hasExtension) override;
  void setCSRCCount(uint8_t CSRCCount) override;
  QByteArray _data;

private:  


};

#endif // RTP_PACKET_H
