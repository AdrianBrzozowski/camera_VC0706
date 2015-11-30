#ifndef SERIAL_PORT_QT_H
#define SERIAL_PORT_QT_H

#include "iserialport.h"

#include <QtSerialPort/QSerialPort>

class SerialPort_Qt : public ISerialPort
{
public:
    SerialPort_Qt();

    int openSP(const char *device, int baud, char parity, int data_bit, int stop_bit);
    int closeSP();

    int writeSP(const uint8_t nb, const uint8_t *req);
    int readSP(uint16_t &nb, uint8_t *resp);

private:
    QSerialPort *serialPort;
    QString serialPortName;

    static const int READ_BUFFER_SIZE = 1;
};

static inline void loadBar(unsigned int x, unsigned int n, unsigned int w = 50)
{
    if ( (x != n) && (x % (n/100+1) != 0) ) return;

    float ratio  =  x/(float)n;
    int   c      =  ratio * w;

    cout << setw(3) << (int)(ratio*100) << "% [";
    for (int x=0; x<c; x++) cout << "=";
    for (int x=c; x<w; x++) cout << " ";
    cout << "]\r" << flush;
}

#endif // SERIAL_PORT_QT_H
