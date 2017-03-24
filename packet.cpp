#include <string.h>

#include "packet.h"
#include "measurement.h"

Packet::Packet(QObject *parent) : QObject(parent)
{
    mModel = (Model*)parent;
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

void Packet::parse()
{
    Measurement *meas;
    uint16_t size = (uint16_t)((mInData->at(3) << 8) | mInData->at(2));
    tPacketCMD cmd = (tPacketCMD)mInData->at(4);
    tDataTypes xdatatype, ydatatype;
    uint16_t id, numpoints;
    QVector<double> *xdata, *ydata;

    switch(cmd)
    {
        case CMD_START:
            mModel->setClientRunning(true);
            qInfo("CMD_START");
            break;
        case CMD_END:
            mModel->setClientRunning(false);
            qInfo("CMD_END");
            break;
        case CMD_DECLARATION:
            qInfo("CMD_DECLARATION");
            meas = new Measurement;
            xdatatype = (tDataTypes)(mInData->at(7));
            ydatatype = (tDataTypes)(mInData->at(8));
            meas->id = (uint16_t)((mInData->at(6) << 8) | mInData->at(5));
            meas->name = QString(mInData->mid(9));
            meas->unit = QString(mInData->mid(9+meas->name.length()+1));
            meas->xDataType = xdatatype;
            meas->yDataType = ydatatype;
            mModel->addMeasurement(meas);
            break;
        case CMD_DATA:
            qInfo("CMD_DATA");
            id = (uint16_t)((mInData->at(6) << 8) | mInData->at(5));
            numpoints = (uint16_t)((mInData->at(8) << 8) | mInData->at(7));
            xdata = parseData(mInData->mid(9, numpoints*mDataTypeSizes[mModel->getMeasurement(id)->xDataType]),
                    mModel->getMeasurement(id)->xDataType,
                    numpoints);
            ydata = parseData(mInData->mid(9+numpoints*mDataTypeSizes[mModel->getMeasurement(id)->xDataType], numpoints*mDataTypeSizes[mModel->getMeasurement(id)->yDataType]),
                    mModel->getMeasurement(id)->yDataType,
                    numpoints);

            break;
//        default:
//            break;
    }
}

QVector<double> *Packet::parseData(QByteArray in, tDataTypes type, int num)
{
    QVector<double> *out = new QVector<double>(num);

    switch(type)
    {
        case TYPE_uint8:
            for(int i = 0; i < num; i++)
            {
                (*out)[i] = ((double)((uint8_t)in.at(i)));
            }
            break;
        case TYPE_int8:
            for(int i = 0; i < num; i++)
            {
                (*out)[i] = ((double)((int8_t)in.at(i)));
            }
            break;
        case TYPE_uint16:

            break;
        case TYPE_int16:

            break;
        case TYPE_uint32:

            break;
        case TYPE_int32:

            break;
        case TYPE_uint64:

            break;
        case TYPE_int64:

            break;
        case TYPE_float:

            break;
        case TYPE_double:

            break;
        default: break;
    }



}
