#ifndef ISERIALPORT_H
#define ISERIALPORT_H

#include <stdint.h>

class ISerialPort
{
public:
    virtual ~ISerialPort(){ }
    virtual int openSP(const char *device, int baud, char parity, int data_bit, int stop_bit) = 0;
    virtual int closeSP() = 0;

    virtual int writeSP(const uint8_t nb, const uint8_t *req) = 0;
    virtual int readSP(uint16_t &nb, uint8_t *resp) = 0;
};

#endif // ISERIALPORT_H
