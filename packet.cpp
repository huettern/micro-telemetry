#include "packet.h"

Packet::Packet(QObject *parent) : QObject(parent)
{
    mState = PACKET_NONE;
    mInData = new QByteArray;
}

/**
 * @brief Add data to the packet
 * @param data
 */
Packet::tPacketStatus Packet::addData (QByteArray data)
{
    bool write = false;
    int i = 0;
    int j = 0;
    switch (mState) {
    case PACKET_NONE:
        // Check for start sequence then save the data
        for(; (i < data.size()) && (write == false); i++)
        {
            if(data[i] == '\xff')
            {
                write = true;
                mState = PACKET_FILLING;
                mPacketSize = 0xffff;
                mInData->append(data[i]);
            }
        }
        // NO BREAK HERE
    case PACKET_FILLING:
        // Check for end sequence then save the data
        for(; (i < data.size()) && (write == true); i++)
        {
            mInData->append(data[i]);
            if(mInData->size() >= 4)
            {
                // we can read the packet total size
                mPacketSize = (((uint8_t)(*mInData)[3])<<8) | ((uint8_t)(*mInData)[2]);
            }
            // Check for end of package
            if(mInData->size() >= mPacketSize)
            {
                mState = PACKET_ENDED;
                write = false;
            }
        }
        // NO BREAK HERE
    case PACKET_ENDED:
        mRemainingData = new QByteArray(NULL, data.size() - i);
        for(; i < data.size(); i++)
        {
            // copy the remaining data
            (*mRemainingData)[j++] = data[i];
        }
        break;
    default:
        break;
    }
    return mState;
}

/**
 * @brief Returns the remaining data if last added Data had more data in it than necessary
 * @return remaining data after addData
 */
QByteArray Packet::getRemainingData ()
{
    return (*mRemainingData);
}
