#include "serialport_fcntl.h"
#include "command.h"

#include <termios.h>
#include <cstring>

SerialPort_fcntl::SerialPort_fcntl()
{
}

int SerialPort_fcntl::openSP(const char *device, int baud, char parity, int data_bit, int stop_bit)
{
    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd != -1)
    {
        fcntl(fd, F_SETFL, 0);

        // setting termisos
        struct termios settings;
        tcgetattr(fd, &settings);

        speed_t newBaud = 0;

        switch (baud)
        {
        case     50:	newBaud =     B50; break;
        case     75:	newBaud =     B75; break;
        case    110:	newBaud =    B110; break;
        case    134:	newBaud =    B134; break;
        case    150:	newBaud =    B150; break;
        case    200:	newBaud =    B200; break;
        case    300:	newBaud =    B300; break;
        case    600:	newBaud =    B600; break;
        case   1200:	newBaud =   B1200; break;
        case   1800:	newBaud =   B1800; break;
        case   2400:	newBaud =   B2400; break;
        case   9600:	newBaud =   B9600; break;
        case  19200:	newBaud =  B19200; break;
        case  38400:	newBaud =  B38400; break;
        case  57600:	newBaud =  B57600; break;
        case 115200:	newBaud = B115200; break;
        case 230400:	newBaud = B230400; break;

        default:
            return 1;
        }

        cfsetispeed(&settings, newBaud);
        cfsetospeed(&settings, newBaud);

        settings.c_cflag &= ~PARENB;
        settings.c_cflag &= ~CSTOPB;
        settings.c_cflag &= ~CSIZE;
        settings.c_cflag |= CS8;
        settings.c_cc[VTIME] = 1;
        settings.c_cc[VMIN] = 0;

        settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        settings.c_iflag &= ~(IXON | IXOFF | IXANY);

        settings.c_oflag &= ~OPOST;

        tcsetattr(fd, TCSANOW, &settings);

    }
    else
    {
        return 1;
    }

    return 0;
}

int SerialPort_fcntl::closeSP()
{
    close(fd);

    return 0;
}

int SerialPort_fcntl::readSP(uint16_t &nb, uint8_t *resp)
{
    int i = 0;
    while (read(fd, &resp[i], 1))
    {
        i++;
        if ((nb > 0) ? (i >= nb) : 0) break;
    }

    nb = i;

#if DEBUG
    printf("Raw read (%d): ", nb);
    for (int j = 0; j < nb; j++) {
        printf("%02x", resp[j]);
    }
    printf("\n");
    fflush(stdout);
#endif

    return 0;
}

int SerialPort_fcntl::writeSP(const uint8_t nb, const uint8_t *req)
{   
    int writeNb = 0;
    for (uint8_t i = 0; i < nb; i++) {
        writeNb += write(fd, &req[i], 1);
    }

#if DEBUG
    printf("Raw write: ");
    for (int i = 0; i < nb; i++) {
        printf("%02x", req[i]);
    }
    printf("\n");
    fflush(stdout);
#endif

    if (writeNb < nb)
        return 1; // TODO: error code

    return 0;
}
