/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "includes.h"
#include "globals.h"
/*
*********************************************************************************************************
*/

void BSP_Init(void)
{
	/* System Clocks Configuration --72M*/

	RCC_Configuration();

	UnableJTAG();
	FLASH_Unlock();
	EE_Init();

	USART1_Configuration(115200);

	NVIC_Configuration();
	GPIO_Configuration();
	EXTI_Configuration();
	MODE_Configruation();
	CAN_Configruation();
//	IWDG_Configruation();


	LED_LED1_OFF();
	LED_LED2_OFF();
	LED_LED3_OFF();

	SIGNAL_DOWN_OFF();
	SIGNAL_UP_OFF();
	SIGNAL_Clu_OFF();

	USART_printf(USART1, "\r\nBoard support package initialization...succeed \r\n");
	USART_printf(USART1, "\r\nRunning in MODE = %d \r\n",uMode);
	switch(uMode)
	{
		case MODE_NORMAL:USART_printf(USART1, "\r\nMODE_NORMAL\r\n");
			break;
		case MODE_ACCEL:USART_printf(USART1, "\r\nMODE_ACCEL\r\n");
			break;
		default:USART_printf(USART1, "\r\nThis Mode is not defined!!! \r\n");
	}
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

////////////////////////////////////////////////////////
void RCC_Configuration(void)
{
	SystemInit();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
							RCC_APB2Periph_GPIOB |
							RCC_APB2Periph_AFIO |
							RCC_APB2Periph_USART1,
							ENABLE);
}

void IWDG_Configruation(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);      /* 写入0x5555,用于允许狗狗寄存器写入功能 */
    IWDG_SetPrescaler(IWDG_Prescaler_256);      /* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
    IWDG_SetReload(781);        /* 喂狗时间 5s/6.4MS=781 .注意不能大于0xfff*/
    IWDG_ReloadCounter();
    IWDG_Enable();

}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

}

void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_11;						//IN 1 2 3  //KEY
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);

	EXTI_InitStructure.EXTI_Line = EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;

	EXTI_Init(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;

	EXTI_Init(&EXTI_InitStructure);


}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;



	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;				                 //LED1 LED2 LED3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;					//OUT S J K
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;											//CAN_EN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void CAN_Configruation()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
        
        
        
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//PB6 Rs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_ResetBits(GPIOB, GPIO_Pin_6);
          
        
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

        
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	CAN_InitStructure.CAN_TTCM=DISABLE; 			 //MCR-TTCM  时间触发通信模式使能
	CAN_InitStructure.CAN_ABOM=DISABLE; 			 //MCR-ABOM  自动离线管理
	CAN_InitStructure.CAN_AWUM=DISABLE; 			 //MCR-AWUM  自动唤醒模式
	CAN_InitStructure.CAN_NART=DISABLE;				 //MCR-NART  禁止报文自动重传	0-自动重传	 1-报文只传一次
	CAN_InitStructure.CAN_RFLM=DISABLE; 			 //MCR-RFLM  接收FIFO 锁定模式	0-溢出时新报文会覆盖原有报文  1-溢出时，新报文丢弃
	CAN_InitStructure.CAN_TXFP = DISABLE;			 //MCR-TXFP  发送FIFO优先级 0-优先级取决于报文标示符 1-优先级取决于发送请求的顺序
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	 //BTR-SILM/LBKM   CAN正常通信模式
	//CAN_InitStructure.CAN_TXFP=DISABLE;
	//CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;			 //BTR-SJW 重新同步跳跃宽度 1个时间单元
	CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;			 //BTR-TS1 时间段1 占用了2个时间单元
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;			 //BTR-TS1 时间段2 占用了3个时间单元
	CAN_InitStructure.CAN_Prescaler =12;

/*	switch(1000)
	{
		case 1000 : CAN_InitStructure.CAN_Prescaler =6;// 1MBps  //BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+2+3)/6=1Mbps
					break;
		case 500 : CAN_InitStructure.CAN_Prescaler =12;
					break;
		case 250 : CAN_InitStructure.CAN_Prescaler =24;
					break;
		case 125 : CAN_InitStructure.CAN_Prescaler =48;
					break;
		case 100 : CAN_InitStructure.CAN_Prescaler =60;
					break;
		case 50 : CAN_InitStructure.CAN_Prescaler =120;
					break;
		case 20 : CAN_InitStructure.CAN_Prescaler =300;
					break;

	}*/


	  CAN_Init(CAN1, &CAN_InitStructure);

	  /* CAN过滤器初始化 */
	  CAN_FilterInitStructure.CAN_FilterNumber=0;						//
	  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//FM1R	过滤器组0的工作模式。
																		//0: 过滤器组x的2个32位寄存器工作在标识符屏蔽位模式；
																		///1:过滤器组x的2个32位寄存器工作在标识符列表模式。
	  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//FS1R 过滤器组0(13～0)的位宽。
																		//0：过滤器位宽为2个16位； 1：过滤器位宽为单个32位。

	  /* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */
          CAN_FilterInitStructure.CAN_FilterIdHigh  = (((u32)ID_ECU<<21)&0xffff0000)>>16;
          CAN_FilterInitStructure.CAN_FilterIdLow   = (((u32)ID_ECU<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xffff;
          CAN_FilterInitStructure.CAN_FilterMaskIdHigh  = 0xFFFF;
          CAN_FilterInitStructure.CAN_FilterMaskIdLow   = 0xFFFF;

	  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;				//FFAx : 过滤器位宽设置 报文在通过了某过滤器的过滤后，
																		//将被存放到其关联的FIFO中。 0：过滤器被关联到FIFO0； 1：过滤器被关联到FIFO1。
	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;				//FACTx : 过滤器激活 软件对某位设置1来激活相应的过滤器。只有对FACTx位清0，
																		//或对CAN_FMR寄存器的FINIT位设置1后，才能修改相应的过滤器寄存器
																		//x(CAN_FxR[0:1])。 0：过滤器被禁用； 1：过滤器被激活。
	  CAN_FilterInit(&CAN_FilterInitStructure);

	  /* CAN FIFO0 接收中断使能 */
	  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

}


void MODE_Configruation(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_10;		//MODE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	uMode = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) | (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) << 1);
}
//关闭调试接口，作GPIO使用
void UnableJTAG(void)
{

 /*  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // enable clock for A
   AFIO->MAPR &= ~(7UL<<24); // clear used bit
   AFIO->MAPR |= (4UL<<24); // set used bits*/
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}


////SYSTICK 配置， 1ms一次systick中断，产生ucos 调度节拍， 1KHz
void  OS_CPU_SysTickInit(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    u32 cnts;
    RCC_GetClocksFreq(&rcc_clocks);
    cnts = (u32)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;

	SysTick_Config(cnts);
}


#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

