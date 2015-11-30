#include "camPTC08.h"
#include "iserialport.h"

#include <iostream>
#include <cstdio>
#include <unistd.h> // sleep
#include <cstring>

CamPTC08::CamPTC08(const char *device, int baud, char parity, int data_bit, int stop_bit) :
    serialPort(NULL),
    device(device),
    baudrate(baud),
    parity(parity),
    dataBits(data_bit),
    stopBits(stop_bit)
{
    req = new uint8_t[VC0706_REQ_MAX_BYTES];
    resp = new uint8_t[VC0706_RESP_MAX_BYTES*2];
    setDefaulParamsLocaly();
}

CamPTC08::~CamPTC08()
{
    delete[] req;
    delete[] resp;
}

int CamPTC08::open()
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    return serialPort->openSP(device.c_str(), baudrate, parity, dataBits, stopBits);
}

int CamPTC08::close()
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    serialPort->closeSP();

    return 0;
}

int CamPTC08::write(uint8_t nb, const uint8_t *req)
{
    serialPort->writeSP(nb, req);

    return 0;
}

int CamPTC08::read(uint16_t &nb, uint8_t *resp)
{
    serialPort->readSP(nb, resp);

    return 0;
}

const char* CamPTC08::getVersion(void)
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    uint8_t cmd = VC0706_GEN_VERSION;
    uint16_t nb = 0;

    sendRequest(cmd, 0, NULL);
    readResponse(nb, resp);
    verifyResponse(cmd, resp);

    return (char*)getResponseData(resp);
}

int CamPTC08::takePhoto(uint8_t fbuf_type /*= VC0706_FBUF_CURRENT*/, std::string path_image)
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    int status = RESP_ERROR;
    uint8_t *imageBuff; // TODO: as external pointer
    uint16_t imageLength;

    status = stopFrame(fbuf_type);
    if (status == RESP_ERROR) {
        return status;
    }
    usleep(100000);

    getFrameLenght(imageLength, fbuf_type);
    if (status == RESP_ERROR) {
        return status;
    }
    usleep(100000);

    // TODO: handle error value of getImageLenght

    imageBuff = new uint8_t[imageLength];
    readFrameBuffer(imageLength, imageBuff, fbuf_type);
    if (status == RESP_ERROR) {
        return status;
    }
    usleep(100000);

    reasumeFrame();
    usleep(100000);

    if (!path_image.empty())
    {
        FILE* f = fopen(path_image.c_str(), "wb");
        if (f == NULL)
        {
            return 1;
        }

        fwrite(imageBuff, sizeof(*imageBuff), imageLength, f);
        fclose(f);
    }

    delete[] imageBuff;

    return status;
}

int CamPTC08::setSerialPort(ISerialPort *serial_port)
{
#if DEBUG
    std::cout << "" << std::endl;
#endif

    serialPort = serial_port;
}

int CamPTC08::setSerialNumber(uint8_t serial_number)
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    uint8_t cmd = VC0706_SET_SERIAL_NUMBER;
    uint16_t nb = 0;
    const uint8_t data[] = {serial_number};

    sendRequest(cmd, 1, data);
    readResponse(nb, resp);
    verifyResponse(cmd, resp);

    this->serialNumber = serial_number;

    return 0;
}

int CamPTC08::systemReset()
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    uint8_t cmd = VC0706_SYSTEM_RESET;
    uint16_t nb = 5;

    sendRequest(cmd, 0, NULL);
    readResponse(nb, resp);
    verifyResponse(cmd, resp);

    setDefaulParamsLocaly();
    usleep(1000000); // from VC0706 documentation

    nb = 0;
    uint8_t basicConfiguration[70]; // TODO: check max count of configuration bytes or flush/ignore this data
    readResponse(nb, basicConfiguration);

    return 0;
}

int CamPTC08::setDefaulParamsLocaly()
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    serialNumber = 0;

    return 0;
}

int CamPTC08::sendRequest(const uint8_t cmd, const uint8_t datan, const uint8_t data[])
{    
    req[0] = VC0706_RECEIVE_SIGN;
    req[1] = serialNumber;
    req[2] = cmd;
    req[3] = datan;

    memcpy(req + VC0706_REQ_MIN_BYTES, data, sizeof(*data) * datan);

    serialPort->writeSP(VC0706_REQ_MIN_BYTES + datan, req);

    return 0;
}

int CamPTC08::verifyResponse(const uint8_t cmd, uint8_t *data)
{
    if ((data[0] != VC0706_RETURN_SIGN) ||
            (data[1] != serialNumber) ||
            (data[2] != cmd) ||
            (data[3] != 0x00))
    {
#if DEBUG
        std::cerr << "The response may contain errors." << std::endl;
#endif
        return RESP_ERROR;
    }

    return RESP_RIGHT;
}

int CamPTC08::readResponse(uint16_t &datan, uint8_t *data)
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    serialPort->readSP(datan, data);

    return 0;
}

unsigned CamPTC08::getRequestStatus()
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    return resp[VC0706_RESP_STATUS_BYTE];
}

const uint8_t* CamPTC08::getResponseData(uint8_t *response)
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    return response + VC0706_RESP_DATA_BYTE;
}

int CamPTC08::getFrameLenght(uint16_t &img_len, uint8_t fbuf_type)
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif
    int status = RESP_ERROR;
    uint8_t cmd = VC0706_GET_FBUF_LEN;
    uint16_t nb = 0;
    const uint8_t data[] = {fbuf_type};

    sendRequest(cmd, 1, data);
    readResponse(nb, resp);
    status = verifyResponse(cmd, resp);

    // TODO: handle if nb < 7

    // int16_t XH = GET_INT16_FROM_INT8(data_resp); // ignoring: max dec value 65534
    img_len = GET_INT16_FROM_INT8((getResponseData(resp)+2));

    return status;
}

int CamPTC08::readFrameBuffer(const uint16_t img_len, uint8_t *img_buff, uint8_t fbuf_type, int nb_chunks)
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    int status = RESP_ERROR;
    uint8_t cmd = VC0706_READ_FBUF;
    uint16_t nb = 0;
    uint32_t startAddress = 0; // TODO: handle it

    const int NB_BITS = 0x0C;
    const uint8_t data[] = {fbuf_type,
                            0x0A, // number of data bytes
                            startAddress>>24, startAddress>>16, startAddress>>8, startAddress & 0xFF, // starting address
                            0x00, 0x00, img_len>>8, img_len & 0xFF, // image data lenght
                            READ_FBUF_DELAY>>8, READ_FBUF_DELAY & 0xFF}; // interval

    // read intro
    nb = 5;
    sendRequest(cmd, NB_BITS, data);
    readResponse(nb, resp);
    status = verifyResponse(cmd, resp);
    if (status == RESP_ERROR) {
        return status;
    }
        usleep(10000);

    // read image
    // TODO: multi reading in small chunks
    nb = img_len;

    for (int i=0; i<nb_chunks; ++i)
    {

        usleep(1000);
        readResponse(nb, img_buff);
        if (i==0) {
            if (img_buff[0] != 0xFF || img_buff[1] != 0xD8) {
#if DEBUG
                std::cerr << "Wrong start of image." << std::endl;
#endif
                return RESP_READ_FBUF_ESI;
            }

        }
        if (i==nb_chunks-1) { // TODO
            if (img_buff[img_len-2] != 0xFF || img_buff[img_len-1] != 0xD9) {
#if DEBUG
                std::cerr << "Wrong end of image." << std::endl;
#endif
                return RESP_READ_FBUF_EEI;
            }
        }
    }
    usleep(10000);

    // read outro
    nb = 5;
    readResponse(nb, resp);
    status = verifyResponse(cmd, resp);
    if (status == RESP_ERROR) {
        return status;
    }

    return status;
}

int CamPTC08::stopFrame(uint8_t fbuf_type)
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif
    int status = RESP_ERROR;

    uint8_t cmd = VC0706_FBUF_CTRL;
    uint16_t nb = 0;
    const uint8_t data[] = {fbuf_type};

    sendRequest(cmd, 1, data);
    readResponse(nb, resp);
    status = verifyResponse(cmd, resp);

    return status;
}

int CamPTC08::reasumeFrame()
{
#if DEBUG
    std::cout << "// call: " << __func__ << std::endl;
#endif

    uint8_t cmd = VC0706_FBUF_CTRL;
    uint16_t nb = 0;
    const uint8_t data[] = {VC0706_FBUF_CTRL_RESUME}; // TODO: check if it value should be here, vs VC0706_FBUF_CTRL_STEP

    sendRequest(cmd, 1, data);
    usleep(10000);
    nb = 5;
    readResponse(nb, resp);
    verifyResponse(cmd, resp);

    return 0;
}
