#ifndef COMMAND_H
#define COMMAND_H

#define GET_INT16_FROM_INT8(tabInt8) ((tabInt8[0] << 8) + tabInt8[1])
#define GET_INT32_FROM_INT16(tabInt16) ((tabInt16[1] << 16) + tabInt16[0])
#define GET_INT32_FROM_INT8(tabInt8) ((tabInt8[3] << 32) + (tabInt8[2] << 16) + (tabInt8[1] << 8) + tabInt8[0])

// commands
#define VC0706_GEN_VERSION 0x11
#define VC0706_SET_SERIAL_NUMBER 0x21
#define VC0706_SET_PORT 0x24
#define VC0706_SYSTEM_RESET  0x26
#define VC0706_READ_DATA 0x30
#define VC0706_WRITE_DATA 0x31
#define VC0706_READ_FBUF 0x32
#define VC0706_WRITE_FBUF 0x33
#define VC0706_GET_FBUF_LEN 0x34
#define VC0706_SET_FBUF_LEN 0x35
#define VC0706_FBUF_CTRL 0x36
#define VC0706_COMM_MOTION_CTRL 0x37
#define VC0706_COMM_MOTION_STATUS 0x38
#define VC0706_COMM_MOTION_DETECTED 0x39

#define VC0706_MOTION_CTRL 0x42
#define VC0706_MOTION_STATUS 0x43
#define VC0706_TV_OUT_CTRL 0x44
#define VC0706_OSD_ADD_CHAR 0x45

#define VC0706_DOWNSIZE_CTRL 0x54
#define VC0706_DOWNSIZE_STATUS 0x55

#define VC0706_RECEIVE_SIGN 0x56
#define VC0706_RETURN_SIGN 0x76

// frame buffer type
#define VC0706_FBUF_CURRENT 0
#define VC0706_FBUF_NEXT 1

// control frame buffer register
#define VC0706_FBUF_CTRL_STOP_CURRENT 0
#define VC0706_FBUF_CTRL_STOP_NEXT 1
#define VC0706_FBUF_CTRL_RESUME 2
#define VC0706_FBUF_CTRL_STEP 3

// protocol volume in bytes
#define VC0706_PROTOCOL_SIGN 1
#define VC0706_SERIAL_NUMBER 1
#define VC0706_COMMAND 1
#define VC0706_DATA_LENGHTS 1
#define VC0706_STATUS 1
#define VC0706_DATA_MAX 16

#define VC0706_REQ_MIN_BYTES VC0706_PROTOCOL_SIGN+VC0706_SERIAL_NUMBER+VC0706_COMMAND+VC0706_DATA_LENGHTS
#define VC0706_REQ_MAX_BYTES VC0706_REQ_MIN_BYTES+VC0706_DATA_MAX

#define VC0706_RESP_MIN_BYTES VC0706_PROTOCOL_SIGN+VC0706_SERIAL_NUMBER+VC0706_COMMAND+VC0706_DATA_LENGHTS+VC0706_STATUS
#define VC0706_RESP_MAX_BYTES VC0706_RESP_MIN_BYTES+VC0706_DATA_MAX

// body of response frame
#define VC0706_RESP_SERIAL_NUMBER_BYTE 1
#define VC0706_RESP_STATUS_BYTE 3
#define VC0706_RESP_DATA_LENGHTS_BYTE 4
#define VC0706_RESP_DATA_BYTE 5

// VC0706 status code
#define VC0706_RESP_STATUS_ECR 0 // Executing command right
#define VC0706_RESP_STATUS_SDRC 1 // System don't receive the command.
#define VC0706_RESP_STATUS_DLE 2 // The data-length is error.
#define VC0706_RESP_STATUS_DFE 3 // Data format error.
#define VC0706_RESP_STATUS_CCNE 4 // The command can not execute now.
#define VC0706_RESP_STATUS_CRBEW 5 // Command received,but executed wrong.

// custom status code
#define RESP_RIGHT 0 // Everything goes ok.
#define RESP_ERROR -1 // Something goes wrong.
#define RESP_READ_FBUF_ESI -2 // Error with data: start of image.
#define RESP_READ_FBUF_EEI -3 // Error with data: end of image.

// config // TODO: export it
#define DEBUG 0
#define READ_FBUF_DELAY 0x0010

#endif // COMMAND_H
