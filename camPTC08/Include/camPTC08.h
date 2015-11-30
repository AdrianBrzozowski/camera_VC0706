#ifndef CAMPTC08_H
#define CAMPTC08_H

#include <string.h>
#include <stdint.h>

#include "command.h"
#include <string>

class ISerialPort;
class CamPTC08
{
public:
    CamPTC08(const char *device, int baud, char parity = 'N', int data_bit = 8, int stop_bit = 1);
    ~CamPTC08();

    int open();
    int close();

    // request info
    unsigned getRequestStatus();
    unsigned getSerialNumber() const { return serialNumber; }
    std::string getDeviceName() const { return device; }

    /// cmera API
    // getters
    const char* getVersion(void);
    int takePhoto(uint8_t fbuf_type = VC0706_FBUF_CURRENT, std::string path_image = "");
    // TODO: int readDataRegister();
    // TODO: int getColorStatus();
    // TODO: int getPowerSaveStatus();
    // TODO: int getAEStatus();
    // TODO: int getDownSizeStatus();
    // TODO: int readLogo();

    // setters
    int setSerialPort(ISerialPort *serial_port);
    int setSerialNumber(uint8_t serial_number);
    int systemReset();
    // TODO: int setPort();
    // TODO: int writeDataRegister();
    // TODO (do ustalania): int writeBufferRegister();
    // TODO: int setFrameLength();
    // TODO: int setColorControl();
    // TODO: int setPowerSaveCtrl();
    // TODO: int setAE();
    // TODO: int setTVOutput();
    // TODO: int addOSDCharacters();
    // TODO: int setDownSize();

private:
    ISerialPort *serialPort;

    uint8_t serialNumber;
    uint8_t *req;
    uint8_t *resp;

    std::string device;
    int baudrate;
    char parity;
    int dataBits;
    int stopBits;

    int setDefaulParamsLocaly();

    int write(const uint8_t nb, const uint8_t *req);
    int read(uint16_t &nb, uint8_t *resp);

    int sendRequest(const uint8_t cmd, const uint8_t datan, const uint8_t data[]);
    int verifyResponse(const uint8_t cmd, uint8_t *data);
    int readResponse(uint16_t &datan, uint8_t *data);

    const uint8_t* getResponseData(uint8_t *response);

    /// camera API
    // getters
    int getFrameLenght(uint16_t &img_len, uint8_t fbuf_type = VC0706_FBUF_CURRENT);
    int readFrameBuffer(const uint16_t img_len, uint8_t *img_buff, uint8_t fbuf_type = VC0706_FBUF_CURRENT, int nb_chunks = 1);

    // setters
    int stopFrame(uint8_t fbuf_type = VC0706_FBUF_CURRENT);
    int reasumeFrame();
};

#endif // CAMPTC08_H
