#include "spi.h"

#define ENABLE_ESAM_PWR         GPIO_WriteLow(GPIOE,GPIO_PIN_5)
#define DISABLE_ESAM_PWR        GPIO_WriteHigh(GPIOE,GPIO_PIN_5)

//δ���Ժ�����
//Ƭѡ����δ��ʼ��
void Spi_Config(void)
{
  SPI_DeInit();
  SPI_Init(SPI_FIRSTBIT_LSB, SPI_BAUDRATEPRESCALER_2, 
           SPI_MODE_SLAVE, SPI_CLOCKPOLARITY_LOW,
           SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, 
           SPI_NSS_SOFT,(uint8_t)0x07);
  SPI_Cmd(ENABLE);
  
  GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW);
}