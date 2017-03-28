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
            mModel->getMeasurement(id)->xdata.append(*xdata);
            mModel->getMeasurement(id)->ydata.append(*ydata);
            mModel->measUpdated(id);
            break;
//        default:
//            break;
    }
}

/**
 * @brief parses the data in the in Array to a QVector<double>
 * @param QBteArray with payload
 * @param measurement data type
 * @param number of data points in array in
 * @return QVector<double> with data points
 */
QVector<double> *Packet::parseData(QByteArray in, tDataTypes type, int num)
{
    QVector<double> *out = new QVector<double>(num);
    uint16_t tmpu16;
    int16_t tmp16;
    uint32_t tmpu32;
    int32_t tmp32;
    uint64_t tmpu64;
    int64_t tmp64;
    int j = 0;

    ttofloat tofloat;
    ttodouble todouble;

    ttofloat ftest;
    ftest.f = 1.0;

    switch(type)
    {
        case TYPE_uint8:
        for(int i = 0; i < num*mDataTypeSizes[type]; i++)
            {
                (*out)[i] = ((double)((uint8_t)in.at(i)));
            }
            break;
        case TYPE_int8:
        for(int i = 0; i < num*mDataTypeSizes[type]; i++)
            {
                (*out)[i] = ((double)((int8_t)in.at(i)));
            }
            break;
        case TYPE_uint16:
            for(int i = 0; i < num; i++)
            {
                tmpu16  = ((uint16_t)in.at(i) & 0x00ff);
                tmpu16 |= (((uint16_t)in.at(++i)) & 0x00ff) << 8;
                (*out)[j++] = (double)(tmpu16);
            }
            break;
        case TYPE_int16:
            for(int i = 0; i < num*mDataTypeSizes[type]; i++)
            {
                tmp16  = (((int16_t)in.at(i) & 0x00ff));
                tmp16 |= (((int16_t)in.at(++i)) & 0x00ff) << 8;
                (*out)[j++] = (double)(tmp16);
            }
            break;
        case TYPE_uint32:
        for(int i = 0; i < num*mDataTypeSizes[type]; i++)
            {
                tmpu32  = (((uint32_t)in.at(i) & 0x000000ff));
                tmpu32 |= (((uint32_t)in.at(++i)) & 0x000000ff) << 8;
                tmpu32 |= (((uint32_t)in.at(++i)) & 0x000000ff) << 16;
                tmpu32 |= (((uint32_t)in.at(++i)) & 0x000000ff) << 24;
                (*out)[j++] = (double)(tmpu32);
            }
            break;
        case TYPE_int32:
        for(int i = 0; i < num*mDataTypeSizes[type]; i++)
            {
                tmp32  = (((int32_t)in.at(i) & 0x000000ff));
                tmp32 |= (((int32_t)in.at(++i)) & 0x000000ff) << 8;
                tmp32 |= (((int32_t)in.at(++i)) & 0x000000ff) << 16;
                tmp32 |= (((int32_t)in.at(++i)) & 0x000000ff) << 24;
                (*out)[j++] = (double)(tmp32);
            }
            break;
        case TYPE_uint64:
        for(int i = 0; i < num*mDataTypeSizes[type]; i++)
            {
                tmpu64  = (((uint64_t)in.at(i) & 0x00000000000000ff));
                tmpu64 |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 8;
                tmpu64 |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 16;
                tmpu64 |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 24;
                tmpu64 |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 32;
                tmpu64 |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 40;
                tmpu64 |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 48;
                tmpu64 |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 56;
                (*out)[j++] = (double)(tmpu64);
            }
            break;
        case TYPE_int64:
        for(int i = 0; i < num*mDataTypeSizes[type]; i++)
            {
                tmp64  = (((int64_t)in.at(i)) & 0x00000000000000ff);
                tmp64 |= (((int64_t)in.at(++i)) & 0x00000000000000ff) << 8;
                tmp64 |= (((int64_t)in.at(++i)) & 0x00000000000000ff) << 16;
                tmp64 |= (((int64_t)in.at(++i)) & 0x00000000000000ff) << 24;
                tmp64 |= (((int64_t)in.at(++i)) & 0x00000000000000ff) << 32;
                tmp64 |= (((int64_t)in.at(++i)) & 0x00000000000000ff) << 40;
                tmp64 |= (((int64_t)in.at(++i)) & 0x00000000000000ff) << 48;
                tmp64 |= (((int64_t)in.at(++i)) & 0x00000000000000ff) << 56;
                (*out)[j++] = (double)(tmp64);
            }
            break;
        case TYPE_float:
        for(int i = 0; i < num*mDataTypeSizes[type]; i++)
            {
            tofloat.byte  = (((uint32_t)in.at(i) & 0x000000ff));
            tofloat.byte |= (((uint32_t)in.at(++i) & 0x000000ff) << 8);
            tofloat.byte |= (((uint32_t)in.at(++i) & 0x000000ff) << 16);
            tofloat.byte |= (((uint32_t)in.at(++i) & 0x000000ff) << 24);
                (*out)[j++] = (double)(tofloat.f);
            }
            break;
        case TYPE_double:
        for(int i = 0; i < num*mDataTypeSizes[type]; i++)
            {
                todouble.byte  = (((uint64_t)in.at(i)) & 0x00000000000000ff);
                todouble.byte |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 8;
                todouble.byte |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 16;
                todouble.byte |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 24;
                todouble.byte |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 32;
                todouble.byte |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 40;
                todouble.byte |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 48;
                todouble.byte |= (((uint64_t)in.at(++i)) & 0x00000000000000ff) << 56;
                (*out)[j++] = todouble.d;
            }
            break;
        default: break;
    }
    return out;
}
