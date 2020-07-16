/*
*********************************************************************************************************
*	                                  
*	ģ������ : I2C��������ģ��
*	�ļ����� : i2c.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ���
*	�޸ļ�¼ :  ��ĳ�����Ͻ��з�װ�Ķ�������Ӧ������
*		�汾��     ����       ����    ˵��
*         V1.0   2020/04/30    WJ
*
*
*********************************************************************************************************
*/

#ifndef _I2C_H
#define _I2C_H

//#include <inttypes.h>

#include "stm8s.h"

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */











    void i2c_Delay(void);
    void i2c_CfgGpio(void);


    void i2c_Start(void);
    void i2c_Stop(void);
    void i2c_SendByte(uint8_t _ucByte);
    uint8_t i2c_ReadByte(void);
    uint8_t i2c_WaitAck(void);
    void i2c_Ack(void);
    void i2c_NAck(void);  
    uint8_t i2c_CheckDevice(uint8_t _Address);
    void i2c_start_set(void);
    void i2c_stop_set(void);
void delay_us();





#endif
