#if !defined( _BSP_H )
#define _BSP_H


void RCC_Configuration(void);

void GPIO_Configuration(void);

void NVIC_Configuration(void);
void EXTI_Configuration(void);

void SPI2_NRF24L01_Init(void);
void NRF24L01_Address_Configuration(void);
void USART_Configuration(void);
void CAN_Configruation(void);
void MODE_Configruation(void);
void IWDG_Configruation(void);
void ADC_Configruation(void);
void RTC_Configuration(void);

void OS_CPU_SysTickInit(void);

void BSP_Init(void);

void UnableJTAG(void);


#endif
