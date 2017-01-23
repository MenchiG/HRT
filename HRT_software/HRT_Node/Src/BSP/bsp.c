/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "includes.h"

/*
*********************************************************************************************************
*/

void BSP_Init(void)
{
	/* System Clocks Configuration --72M*/
	RCC_Configuration();
	NVIC_Configuration();
	GPIO_Configuration();
	CAN_Configruation();
	ADC_Configruation();
	USART1_Configuration(115200);
	MODE_Configruation();
	TIM_Configuration();
//	IWDG_Configruation();
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

////////////////////////////////////////////////////////
void IWDG_Configruation(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);      /* 写入0x5555,用于允许狗狗寄存器写入功能 */
    IWDG_SetPrescaler(IWDG_Prescaler_256);      /* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
    IWDG_SetReload(781);        /* 喂狗时间 5s/6.4MS=781 .注意不能大于0xfff*/
    IWDG_ReloadCounter();
    IWDG_Enable();

}

void RCC_Configuration(void)
{
	SystemInit();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_TIM4
						  ,ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1 |
						   RCC_APB2Periph_ADC1
						   ,ENABLE);


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1
							,ENABLE);
}

void MODE_Configruation(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;		//MODE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	can_address = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) | (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) << 1);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;							//PA12 LED

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


}
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);


	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 15;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);


}


void CAN_Configruation(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//PB8 CAN_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//PB9 CAN_TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//PA5 Rs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
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

	/*switch(500)
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
      CAN_FilterInitStructure.CAN_FilterIdHigh   = (((u32)node_id<<21)&0xffff0000)>>16;
      CAN_FilterInitStructure.CAN_FilterIdLow   = (((u32)node_id<<21)|CAN_ID_STD)&0xffff;
      CAN_FilterInitStructure.CAN_FilterMaskIdHigh  = 0xFFFF;
      CAN_FilterInitStructure.CAN_FilterMaskIdLow   = 0xFFFC;
	  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;		//FFAx : 过滤器位宽设置 报文在通过了某过滤器的过滤后，
																		//将被存放到其关联的FIFO中。 0：过滤器被关联到FIFO0； 1：过滤器被关联到FIFO1。
	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;				//FACTx : 过滤器激活 软件对某位设置1来激活相应的过滤器。只有对FACTx位清0，
																		//或对CAN_FMR寄存器的FINIT位设置1后，才能修改相应的过滤器寄存器
																		//x(CAN_FxR[0:1])。 0：过滤器被禁用； 1：过滤器被激活。
	  CAN_FilterInit(&CAN_FilterInitStructure);

	  /* CAN FIFO0 接收中断使能 */
	  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

}
void ADC_Configruation(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);



    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x4001244C;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_N*ADC_M;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //循环模式开启，Buffer写满后，自动回到初始地址开始传输
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    //配置完成后，启动DMA通道
    DMA_Cmd(DMA1_Channel1, ENABLE);

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);



    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //连续转换开启
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 4;     //设置转换序列长度为4
    ADC_Init(ADC1, &ADC_InitStructure);


    //常规转换序列1：通道10
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_239Cycles5);

    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
    ADC_DMACmd(ADC1, ENABLE);

    // 下面是ADC自动校准，开机后需执行一次，保证精度
    // Enable ADC1 reset calibaration register
    ADC_ResetCalibration(ADC1);
    // Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));

    // Start ADC1 calibaration
    ADC_StartCalibration(ADC1);
    // Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
    // ADC自动校准结束---------------
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}


void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = 115200;//波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据位数
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No ;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件控制流
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送接收均使能

    /* Configure the USARTx */
    USART_Init(USART1, &USART_InitStructure);
    /* Enable the USARTx */
    USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);


}



void TIM_Configuration(void)
{
    TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

   // TIM4 时基
    TIM_DeInit(TIM4);
    TIM_TimeBaseStructure.TIM_Period = 0xffff;                                  //自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = 719;                                   //预分频值, 使TIMx_CLK=1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                     //输入时钟不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //向上计数

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
                             //TIM4_TimeBase
// TIM_ICInitStructure.TIM_ICMode = TIM_ICMode_ICAP;                             //输入捕捉方式
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;								//输入通道
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;					//捕捉上升沿
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;				//捕捉中断
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;							//捕捉不分频
  TIM_ICInitStructure.TIM_ICFilter = 0x0;										//捕捉输入不滤波

  TIM_ICInit(TIM4, &TIM_ICInitStructure);

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2 ;								//输入通道
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;					//捕捉上升沿
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;				//捕捉中断
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;							//捕捉不分频
  TIM_ICInitStructure.TIM_ICFilter = 0x0;										//捕捉输入不滤波


  /* TIM enable counter */
  TIM_Cmd(TIM4, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
  TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
}


//关闭调试接口，作GPIO使用
void UnableJTAG(void)
{
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

