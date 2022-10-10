#ifndef DATA_PROTOCOL_H
#define DATA_PROTOCOL_H
#define MAX_LENGTH 256

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include"usart.h"
static uint8_t sendBuff[MAX_LENGTH];
 enum DeviceCmd
{
	Read_Coil=0x80,//读线圈
	Write_Coil=0x81,//写线圈
	Read_Reg=0x82,//读寄存器
	Write_Reg=0x83//写寄存器
};
 enum CmdType{
	USART_TYPE=0x01,
	I2C_TYPE=0x02,
	SPI_TYPE=0x03
};	
typedef struct MyProtocol
{
	uint8_t  FrameHeader;	    //帧头 Byte
	uint16_t  DeviceAddress;					//设备地址
	enum DeviceCmd  Cmd;						//指令/命令
	enum CmdType  CmdType;				//命令类型/功能码
	uint16_t  DataLength;				//数据长度
	uint8_t  Data[MAX_LENGTH];		//数据
	uint16_t  ChecksumLBit;		//校验码
}MyProtocol;

extern MyProtocol Protocol;
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen);
void InvertUint8(unsigned char *DesBuf, unsigned char *SrcBuf);
void InvertUint16(unsigned short *DesBuf, unsigned short *SrcBuf);
void MyProtocolPacket(MyProtocol* Protocol,uint16_t  DeviceAddress,enum DeviceCmd  Cmd,enum CmdType  CmdType,uint8_t  m_Data[MAX_LENGTH]);
void MyProtocolwriteCoil(MyProtocol* Protocol,uint8_t  write_coil_Data[MAX_LENGTH])	;
void usartSend();
void sendProtocolFunc(MyProtocol* Protocol,UART_HandleTypeDef *huart);
void test(uint8_t  Data[MAX_LENGTH]);
#ifdef __cplusplus
}
#endif
#endif
//ifndef _SENSOR_PROTOCOL_H