/*
*********************************************************************************************************
*	                                  
*	模块名称 : I2C总线驱动模块
*	文件名称 : i2c.h
*	版    本 : V1.0
*	说    明 : 头文件。
*	修改记录 :  在某例程上进行封装改动，以适应本工程
*		版本号     日期       作者    说明
*         V1.0   2020/04/30    WJ
*
*
*********************************************************************************************************
*/

#ifndef _I2C_H
#define _I2C_H

//#include <inttypes.h>

#include "stm8s.h"

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */











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
