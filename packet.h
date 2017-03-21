#ifndef PACKET_H
#define PACKET_H

#include <QObject>

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

    tPacketStatus addData (QByteArray data);
    QByteArray getRemainingData ();

private:
    QByteArray *mInData;
    QByteArray *mRemainingData;
    tPacketStatus mState;

    uint16_t mPacketSize;



signals:

public slots:
};

#endif // PACKET_H
