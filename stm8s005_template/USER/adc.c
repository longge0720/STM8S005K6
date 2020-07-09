#include "adc.h"

//Œ¥≤‚ ‘∫Õ“˝”√
void ADC_Config(void)
{
  /*  Init GPIO for ADC1 */
  GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_IN_FL_NO_IT);//ac_check
  GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);//ac_check2
  /* De-Init ADC peripheral*/
  ADC1_DeInit();

  /* Init ADC2 peripheral */
   ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_0, 
             ADC1_PRESSEL_FCPU_D18, ADC1_EXTTRIG_TIM, 
             DISABLE, ADC1_ALIGN_RIGHT,
             ADC1_SCHMITTTRIG_CHANNEL0, ENABLE);

  /* Enable EOC interrupt */
//  ADC1_ITConfig(ADC1_IT_EOC,ENABLE);
 
  /*Start Conversion */
//  ADC1_StartConversion();
   ADC1_Cmd(ENABLE);
}
