#ifndef IRTP_PACKET_H
#define IRTP_PACKET_H

#include <QByteArray>
#include <QList>

class IRTP_Packet
{
public:
  virtual ~IRTP_Packet() {}

  virtual uint8_t version() const =0;
  virtual void setVersion(uint8_t version) =0;
  virtual bool hasPadding() const =0;
  virtual void setHasPadding(bool hasPadding) =0;
  virtual bool hasExtension() const =0;
  virtual uint8_t CSRCCount() const =0;
  virtual bool hasMarker() const =0;
  virtual void setHasMarker(bool hasMarker) =0;
  virtual uint8_t payloadType() const =0;
  virtual void setPayloadType(uint8_t payloadType) =0;
  virtual uint16_t sequence() const =0;
  virtual void setSequence(uint16_t sequence) =0;
  virtual uint32_t timestamp() const =0;
  virtual void setTimestamp(uint32_t timestamp) =0;
  virtual uint32_t SSRC() const =0;
  virtual void setSSRC(uint32_t SSRC) =0;
  virtual QList<uint32_t> CSRC() const =0;
  virtual void setCSRC(const QList<uint32_t> &CSRC) =0;

  virtual uint16_t extensionHeaderId() const =0;
  virtual uint16_t extensionHeaderSize() const =0;
  virtual QByteArray extensionHeaderData() const =0;
  virtual void setExtensionHeader(uint16_t id, const QByteArray &data) =0;
  virtual uint8_t *payloadData() const =0;
  virtual uint32_t setPayloadData(uint8_t* data, uint32_t size) =0;

  virtual QByteArray const &data() const =0;
  virtual QByteArray &data() =0;
  virtual uint8_t * rawData() const =0;
  virtual void setData(const QByteArray &data) =0;

  virtual uint16_t payloadStart() const =0;
  virtual uint64_t payloadLength() const =0;
  virtual uint64_t size() const =0;

protected:
  virtual void setHasExtension(bool hasExtension) =0;
  virtual void setCSRCCount(uint8_t CSRCCount) =0;

};

#endif // IRTP_PACKET_H
