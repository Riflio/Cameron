#ifndef RTP_PACKET_H
#define RTP_PACKET_H

#include "Interfaces/irtp_packet.h"
#include <QByteArray>
#include <QDebug>
/**
* @brief Пакет RTP
*/
class RTP_Packet: public IRTP_Packet
{
public:
  RTP_Packet(const QByteArray &data);
  virtual ~RTP_Packet() {}

  int getPayloadStart() const;
  int getPayloadLength() const;

  bool hasPadding() const;

  BYTE getCC() const;
  unsigned int getTimeStamp() const;
  unsigned short getSequence() const;
  const int RTP_HEADER_SIZE = 12;

  QByteArray data() const;
  void setData(const QByteArray &data);

  RTP_Packet& operator=(const QByteArray &data);
  RTP_Packet& operator=(const RTP_Packet &packet);

private:
  QByteArray _data;

};

#endif // RTP_PACKET_H
