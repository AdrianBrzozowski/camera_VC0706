#ifndef SERIAL_PORT_FCNTL
#define SERIAL_PORT_FCNTL

#include "iserialport.h"

#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

class SerialPort_fcntl : public ISerialPort
{
public:
    SerialPort_fcntl();

    int openSP(const char *device, int baud, char parity, int data_bit, int stop_bit);
    int closeSP();

    int writeSP(const uint8_t nb, const uint8_t *req);
    int readSP(uint16_t &nb, uint8_t *resp);

private:
    int fd;
};

#endif // SERIAL_PORT_FCNTL
