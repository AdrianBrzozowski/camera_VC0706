#include "serialport_qt.h"
#include "command.h"

#include <QFile>
#include <QBuffer>

#include <iostream>

static inline void loadBar(unsigned int x, unsigned int n, unsigned int w = 50)
{
    if ( (x != n) && (x % (n/100+1) != 0) ) return;

    float ratio  =  x/(float)n;
    int   c      =  ratio * w;

    std::cout << (int)(ratio*100) << "% [";
    for (int x=0; x<c; x++) std::cout << "=";
    for (int x=c; x<w; x++) std::cout << " ";

    std::cout << "]";

    if (x == n) {
        std::cout << "\n" << std::flush;
    } else {
        std::cout << "\r" << std::flush;
    }
}

SerialPort_Qt::SerialPort_Qt()
{
}

int SerialPort_Qt::openSP(const char *device, int baud, char parity, int data_bit, int stop_bit)
{
    serialPort = new QSerialPort;
    serialPort->setPortName(device);
    serialPort->setBaudRate(baud);

    if (!serialPort->open(QIODevice::ReadWrite))
    {
        return 1;
    }

    return 0;
}

int SerialPort_Qt::closeSP()
{
    serialPort->close();

    return 0;
}

int SerialPort_Qt::readSP(uint16_t &nb, uint8_t *resp)
{
    QByteArray readData;
    char buffer[READ_BUFFER_SIZE];
    bool specyficRead = (nb > 0) ? true : false;
    bool isAvaibleData = serialPort->waitForReadyRead(50);
    int receivedBytes = 0;

    while (isAvaibleData || specyficRead) {
        if (nb > 0) {
            receivedBytes +=serialPort->read(buffer, READ_BUFFER_SIZE);
            readData.append(buffer, READ_BUFFER_SIZE);
#if DEBUG
            loadBar(receivedBytes, nb);
#endif
            if (receivedBytes >= nb) {
                specyficRead = false;
                break;
            } else {
                if (isAvaibleData == false) isAvaibleData = serialPort->waitForReadyRead(333);
            }
        }
        else
            readData.append(serialPort->readAll());

        isAvaibleData = serialPort->waitForReadyRead(50);
    }

    if (serialPort->error() == QSerialPort::ReadError)
    {
        std::cerr << "Failed to read from port " << serialPortName.toStdString() << ", error: " << serialPort->errorString().toStdString() << std::endl;
        return 1;
    } else if (serialPort->error() == QSerialPort::TimeoutError && readData.isEmpty()) {
        std::cerr << "No data was currently available for reading from port " << serialPortName.toStdString() << std::endl;
        return 1;
    }

#if DEBUG
    std::cout << "Raw read: " <<  readData.toHex().toStdString() << std::endl;
#endif

    if (nb == 0) {
        nb = readData.size();
    }

    memcpy(resp, readData, sizeof(*resp)*nb);

    return 0;
}

int SerialPort_Qt::writeSP(const uint8_t nb, const uint8_t *req)
{
    QByteArray writeData = QByteArray::fromRawData((char*)req, nb);

    if (writeData.isEmpty())
    {
        std::cout << "No data was currently available on the input for writing to port " << serialPortName.toStdString() << ", error: " << serialPort->errorString().toStdString() << std::endl;
        return 1;
    }

    qint64 bytesWritten = serialPort->write(writeData);

    if (bytesWritten == -1)
    {
        std::cout << "Failed to write the data to port " << serialPortName.toStdString() << ", error: " << serialPort->errorString().toStdString() << std::endl;
        return 1;
    } else if (bytesWritten != writeData.size()) {
        std::cout << "Failed to write all the data to port " << serialPortName.toStdString() << ", error: " << serialPort->errorString().toStdString() << std::endl;
        return 1;
    } else if (!serialPort->waitForBytesWritten(5000)) {
        std::cout << "Operation timed out or an error occurred for port " << serialPortName.toStdString() << ", error: " << serialPort->errorString().toStdString() << std::endl;
        return 1;
    }


#if DEBUG
    std::cout << "Raw write: " <<  writeData.toHex().toStdString() << std::endl;
#endif

    return 0;
}
