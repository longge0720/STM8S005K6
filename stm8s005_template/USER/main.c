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
//电机状态，分合闸状态，开关状态
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


static void aCLK_Config(void)
{
    CLK_DeInit();//寄存器恢复默认
    CLK_HSECmd(ENABLE);//使能外部时钟
    //while(!(CLK->ECKR & (0x01<<1))); //等待时钟就绪
    CLK_ClockSwitchCmd(ENABLE);//允许切换时钟
    /* Configure the Fcpu to DIV1*/
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);//配置CPU分频
    
    /* Configure the HSI prescaler to the optimal value */
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    /* Configure the system clock to use HSE clock source and to run at 24Mhz */
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_MANUAL, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
    /* Output Fcpu on CLK_CCO pin */
    CLK_CCOConfig(CLK_OUTPUT_CPU);
    

    while(1)
    {
       int i=0;
       int j=0;
       int k=0;
       
//       GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);
//       GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);//,GPIO_MODE_OUT_PP_LOW_FAST
//          

          
         GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);//亮
         
         
         GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);//灭
         for(k=0;k<10;k++)
         {
                    for(j=0;j<254;j++)
                   {
                     for(i=0;i<254;i++);
                   }
           
         }

           GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);//灭
                  for(k=0;k<10;k++)
         {
                    for(j=0;j<254;j++)
                   {
                     for(i=0;i<254;i++);
                   }
           
         }
          
    
    }

}
void main(void)
{
  aCLK_Config();
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
  
  for(int i=0;i<8;i++)//上电延时4s,规约要求
  {
    Delay_MS(500);//使用此延时，必须先开启定时器
    Led1_Reverse();
  }
  LED_BRIGHT();//默认保持电量状态
  
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
    
#if 0  //进入低功耗测试
    if(IsLowPower == 1)//进入低功耗模式，IsLowPower还未定义，收到命令运行10秒进入低功耗？
    {
      EnterLowPower();
    }
    else //正常运行模式
    {
      Key_Driver();
      Uart_Driver();
      Motor_Driver();
    }

#endif
//    IWDG_ReloadCounter();//喂狗
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

