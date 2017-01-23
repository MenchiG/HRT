#ifndef _BSP_H_
#define _BSP_H_


void RCC_Configuration(void);
void NVIC_Configuration(void);

void GPIO_Configuration(void);
void ADC_Configruation(void);
void USART_Configuration(void);
void CAN_Configruation(void);
void MODE_Configruation(void);
void IWDG_Configruation(void);
void TIM_Configuration(void);

void OS_CPU_SysTickInit(void);

void BSP_Init(void);


#endif
