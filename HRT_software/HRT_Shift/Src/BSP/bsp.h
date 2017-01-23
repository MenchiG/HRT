#if !defined( _BSP_H )
#define _BSP_H


void BSP_Init(void);

void RCC_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);
void GPIO_Configuration(void);
void CAN_Configruation(void);
void MODE_Configruation(void);
void IWDG_Configruation(void);

void UnableJTAG(void);

void OS_CPU_SysTickInit(void);



#endif
