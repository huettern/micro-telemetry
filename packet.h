#ifndef PACKET_H
#define PACKET_H

#include <QObject>

#include "model.h"

class Packet : public QObject
{
    Q_OBJECT
public:
    explicit Packet(QObject *parent = 0);

    typedef enum PacketStatus {
        PACKET_NONE,
        PACKET_STARTED,
        PACKET_FILLING,
        PACKET_ENDED,
    } tPacketStatus;

    typedef enum {
        TYPE_uint8 = 0,
        TYPE_int8 = 1,
        TYPE_uint16 = 2,
        TYPE_int16 = 3,
        TYPE_uint32 = 4,
        TYPE_int32 = 5,
        TYPE_uint64 = 6,
        TYPE_int64 = 7,
        TYPE_float = 8,
        TYPE_double = 9
    } tDataTypes;

    tPacketStatus addData (QByteArray data);
    QByteArray getRemainingData ();
    void parse();

private:
    Model *mModel;
    QByteArray *mInData;
    QByteArray *mRemainingData;
    tPacketStatus mState;

    uint16_t mPacketSize;

    typedef enum {
        CMD_START = 0x00,
        CMD_END = 0x01,
        CMD_DECLARATION = 0x02,
        CMD_DATA = 0x03,
    } tPacketCMD;

    const uint8_t mDataTypeSizes[10] =
      { 1, 1, 2, 2, 4, 4, 8, 8, 4, 8};

    QVector<double> *parseData(QByteArray in, tDataTypes type, int num);

signals:

public slots:
};

#endif // PACKET_H
