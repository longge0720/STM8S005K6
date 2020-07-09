/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "config.h"
    
#include "uart2.h"
#include "motor.h"
#include "tim4.h"
#include "key.h"
#include "led.h"
#include "hall.h"

#include "iwatchdog.h"
#include "lowpower.h"
#include "device_info.h"
#include "bluetooth.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#define VERSION         "VERSION V1.0\r\n"
    
extern uint8_t MotorState;
//���״̬���ֺ�բ״̬������״̬
static void CLK_Config(void)
{
  CLK_DeInit();
  
  /* Clock divider to HSI/1 */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
  /* Output Fcpu on CLK_CCO pin */
  //CLK_CCOConfig(CLK_OUTPUT_MASTER);
}
void Delay(uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
volatile uint8_t HallSta = 0;
uint8_t backup = 0;
void main(void)
{
  /* Infinite loop */
  CLK_Config();
  
  Led1_Init();
//  Uart2_Init();
  Motor_Init();
  TIM4_Config();
  Key_Init();
  Hall_Init();
    
//  ENABLE_SWITCH_POWER;
  
  ReadDeviceInfo();
  
//  IWDG_Config();
  enableInterrupts();

  BlueTooth_Init();
  
//  printf("uart2 init abcd\r\n");
//  printf("system version %#lX,reset cnt %d ,devid %#02X %#02X %#02X %#02X %#02X %#02X\r\n",dev_info.version,dev_info.cnt,
//       dev_info.dev_id[0],dev_info.dev_id[1],dev_info.dev_id[2],dev_info.dev_id[3],dev_info.dev_id[4],dev_info.dev_id[5]);
  
  for(int i=0;i<8;i++)//�ϵ���ʱ4s,��ԼҪ��
  {
    Delay_MS(500);//ʹ�ô���ʱ�������ȿ�����ʱ��
    Led1_Reverse();
  }
  LED_BRIGHT();//Ĭ�ϱ��ֵ���״̬
  
  while (1)
  {
#if 0
    HallSta = GetHall_State();
    if( HallSta!= backup)
    {
      switch(HallSta)
      {
        case 0:Uart2_SendStr("HALL: U8 L  U9 L\r\n");break;
        case 1:Uart2_SendStr("HALL: U8 L  U9 H\r\n");break;
        case 2:Uart2_SendStr("HALL: U8 H  U9 L\r\n");break;
        default:break;
      }
      backup = HallSta;
    }
#endif 
    
    Key_Driver();
    Uart_Driver();
    Motor_Driver();
   
#if 0
    ENABLE_SWITCH_POWER;
    Delay_MS(5000);
    DISABLE_SWITCH_POWER;
    Delay_MS(5000);
#endif
    
#if 0  //����͹��Ĳ���
    if(IsLowPower == 1)//����͹���ģʽ��IsLowPower��δ���壬�յ���������10�����͹��ģ�
    {
      EnterLowPower();
    }
    else //��������ģʽ
    {
      Key_Driver();
      Uart_Driver();
      Motor_Driver();
    }

#endif
//    IWDG_ReloadCounter();//ι��
  }
  
}



#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

