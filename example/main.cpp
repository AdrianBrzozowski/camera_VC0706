#include "camPTC08.h"

/* Uncomment next line if using fcntl */
#include "serialport_fcntl.h"

/* Uncomment next line if using qt */
/* #include "serialport_qt.h" */

#ifdef USE_QtSerialPort
    #include <QCoreApplication>
#endif

#include <iostream>

int main(int argc, char *argv[])
{
#ifdef USE_QtSerialPort
    QCoreApplication a(argc, argv);
#endif

    CamPTC08 camif("/dev/ttyUSB0", 38400);
    SerialPort_fcntl serialport;
    camif.setSerialPort(&serialport);

    if (camif.open() != 0) {
        std::cerr << "Connection failed: " << camif.getDeviceName() << std::endl;
        return -1;
    }

    std::cout << "S/N: " << camif.getSerialNumber() << std::endl;

//    camif.systemReset();
//    std::cout << "status: " << camif.getRequestStatus() << std::endl;

    std::cout << camif.getVersion() << std::endl;
    std::cout << "status: " << camif.getRequestStatus() << std::endl;

    if (camif.takePhoto(VC0706_FBUF_CURRENT, "photo_1.jpg") != 0) {
        std::cerr << "Problem with takePhoto()" << std::endl;
    }


    camif.getVersion();
    std::cout << "status: " << camif.getRequestStatus() << std::endl;

    if (camif.takePhoto(VC0706_FBUF_CURRENT, "photo_2.jpg") != 0) {
        std::cerr << "Problem with takePhoto()" << std::endl;
    }

    camif.setSerialNumber(33);
    std::cout << "status: " << camif.getRequestStatus() << std::endl;

    camif.getVersion();
    std::cout << "status: " << camif.getRequestStatus() << std::endl;

    camif.setSerialNumber(0);
    camif.close();

    return 0;
}

