#include"data_protocol.h"
MyProtocol Protocol;


/**
**************************************************************************************************
* @Brief    Single byte data inversion
* @Param
*            @DesBuf: destination buffer
*            @SrcBuf: source buffer
* @RetVal    None
* @Note      (MSB)0101_0101 ---> 1010_1010(LSB)
**************************************************************************************************
*/
void InvertUint8(unsigned char *DesBuf, unsigned char *SrcBuf)
{
    int i;
    unsigned char temp = 0;
 
    for(i = 0; i < 8; i++)
    {
        if(SrcBuf[0] & (1 << i))
        {
            temp |= 1<<(7-i);
        }
    }
    DesBuf[0] = temp;
}
 
/**
**************************************************************************************************
* @Brief    double byte data inversion
* @Param
*            @DesBuf: destination buffer
*            @SrcBuf: source buffer
* @RetVal    None
* @Note      (MSB)0101_0101_1010_1010 ---> 0101_0101_1010_1010(LSB)
**************************************************************************************************
*/
void InvertUint16(unsigned short *DesBuf, unsigned short *SrcBuf)
{
    int i;
    unsigned short temp = 0;
 
    for(i = 0; i < 16; i++)
    {
        if(SrcBuf[0] & (1 << i))
        {
            temp |= 1<<(15 - i);
        }
    }
    DesBuf[0] = temp;
}
 
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)
{
    unsigned short wCRCin = 0xFFFF;
    unsigned short wCPoly = 0x8005;
    unsigned char wChar = 0;
 
    while (usDataLen--)
    {
        wChar = *(puchMsg++);
        InvertUint8(&wChar, &wChar);
        wCRCin ^= (wChar << 8);
 
        for(int i = 0; i < 8; i++)
        {
            if(wCRCin & 0x8000)
            {
                wCRCin = (wCRCin << 1) ^ wCPoly;
            }
            else
            {
                wCRCin = wCRCin << 1;
            }
        }
    }
    InvertUint16(&wCRCin, &wCRCin);
    return (wCRCin) ;
}

void MyProtocolPacket(MyProtocol* Protocol,uint16_t  DeviceAddress,enum DeviceCmd  Cmd,enum CmdType  CmdType,uint8_t  m_Data[MAX_LENGTH])
{
    //A1 00 01 00 80 01 00 01 AA AA AA 
    Protocol->FrameHeader=0xA1;
    Protocol->DeviceAddress=DeviceAddress;
    Protocol->Cmd=Cmd;
    Protocol->CmdType=CmdType;
    Protocol->DataLength=MAX_LENGTH;
    for(int i=0;i<MAX_LENGTH;i++)
    {
        Protocol->Data[i]=m_Data[i];
    }
    CRC16_MODBUS(Protocol->Data, Protocol->DataLength);
}
void MyProtocolwriteCoil(MyProtocol* Protocol,uint8_t  write_coil_Data[MAX_LENGTH])
{
    uint16_t  DeviceAddress=0x0001;
    enum DeviceCmd  Cmd=Read_Coil;
    enum CmdType  CmdType=USART_TYPE;

  MyProtocolPacket( Protocol, DeviceAddress, Cmd, CmdType, write_coil_Data);

}
void test(uint8_t  Data[MAX_LENGTH])
{
    for (int i = 0; i < 10; i++)
    {
        /* code */
        Data[i]=0xAA;
    }
    for (int j = 10; j < MAX_LENGTH; j++)
    {
        /* code */
        Data[j]=0xBA;
    }

    
}
void sendProtocolFunc(MyProtocol* Protocol,uint8_t  ProcolBuf[MAX_LENGTH],UART_HandleTypeDef *huart)
{
    memcpy(ProcolBuf, &Protocol, sizeof(MyProtocol));
    int length=sizeof(ProcolBuf) / sizeof(ProcolBuf[0]);
    HAL_UART_Transmit_IT(huart,ProcolBuf,sizeof(ProcolBuf));
    
}
void usartSend()
{
    uint8_t data[MAX_LENGTH]={0};
    test(data);
    MyProtocol *protocol=(MyProtocol*)malloc(sizeof(MyProtocol));
    MyProtocolwriteCoil(protocol,data);
    sendProtocolFunc(protocol,data,USART1);
}