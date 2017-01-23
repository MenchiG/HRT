/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include"includes.h"

/*
*********************************************************************************************************
*/
extern void SPI1_NRF24L01_Init(void);

void BSP_Init(void)
{


	/* System Clocks Configuration --72M*/
	RCC_Configuration();
	UnableJTAG();

	NVIC_Configuration();



	GPIO_Configuration();
	EXTI_Configuration();
	CAN_Configruation();
	USART_Configuration();
	NRF24L01_Address_Configuration();
	SPI1_NRF24L01_Init();
	MODE_Configruation();
	ADC_Configruation();
	RTC_Configuration();
	//IWDG_Configruation();

	MMA_set_mode(MMA_RANGE105);
}

void IWDG_Configruation(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);      /* д��0x5555,�����������Ĵ���д�빦�� */
    IWDG_SetPrescaler(IWDG_Prescaler_256);      /* ����ʱ�ӷ�Ƶ,40K/256=156HZ(6.4ms)*/
    IWDG_SetReload(781);        /* ι��ʱ�� 5s/6.4MS=781 .ע�ⲻ�ܴ���0xfff*/
    IWDG_ReloadCounter();
    IWDG_Enable();

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
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1   | RCC_APB1Periph_SPI2  | RCC_APB1Periph_USART2 |RCC_APB1Periph_PWR   |
  						 RCC_APB1Periph_BKP
  						, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO   | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB  | RCC_APB2Periph_SPI1 |
  						 RCC_APB2Periph_USART1 | RCC_APB2Periph_ADC1
  						 , ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1
						,ENABLE);
}


void MODE_Configruation(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;		//MODE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	can_address = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) | (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) << 1);
}


void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					//g_select
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					//SF
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					//LED
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}



void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = 115200;//������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����λ��
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No ;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���ͽ��վ�ʹ��

    /* Configure the USARTx */
    USART_Init(USART2, &USART_InitStructure);
    /* Enable the USARTx */
    USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
////////////////////////////////////////////////////////////////////////UART1 GPRS



	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 4800;//������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����λ��
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No ;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���ͽ��վ�ʹ��

    /* Configure the USARTx */
    USART_Init(USART1, &USART_InitStructure);
    /* Enable the USARTx */
    USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	Select_GPS();
////////////////////////////////////////////////////////////////////////GPS-Link
}


void NVIC_Configuration(void)
{
  /*  �ṹ����*/

  	NVIC_InitTypeDef NVIC_InitStructure;


  	/* ���ȼ��� 1  */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;					//NRF24L01 �ж���Ӧ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		    //��ռ���ȼ� 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ��
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn ;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

}
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6); 	 //NRF24L01 IRQ  PB2

	EXTI_InitStructure.EXTI_Line = EXTI_Line6;						 //NRF24L01 IRQ PB2
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 			 //EXTI�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 		 //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						 //ʹ��
	EXTI_Init(&EXTI_InitStructure);

}

void CAN_Configruation(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//PA11 CAN_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//PA12 CAN_TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	CAN_DeInit(CAN1);

	CAN_StructInit(&CAN_InitStructure);

	CAN_InitStructure.CAN_TTCM=DISABLE; 			 //MCR-TTCM  ʱ�䴥��ͨ��ģʽʹ��
	CAN_InitStructure.CAN_ABOM=DISABLE; 			 //MCR-ABOM  �Զ����߹���
	CAN_InitStructure.CAN_AWUM=DISABLE; 			 //MCR-AWUM  �Զ�����ģʽ
	CAN_InitStructure.CAN_NART=DISABLE;				 //MCR-NART  ��ֹ�����Զ��ش�	0-�Զ��ش�	 1-����ֻ��һ��
	CAN_InitStructure.CAN_RFLM=DISABLE; 			 //MCR-RFLM  ����FIFO ����ģʽ	0-���ʱ�±��ĻḲ��ԭ�б���  1-���ʱ���±��Ķ���
	CAN_InitStructure.CAN_TXFP = DISABLE;			 //MCR-TXFP  ����FIFO���ȼ� 0-���ȼ�ȡ���ڱ��ı�ʾ�� 1-���ȼ�ȡ���ڷ��������˳��
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	 //BTR-SILM/LBKM   CAN����ͨ��ģʽ
	//CAN_InitStructure.CAN_TXFP=DISABLE;
	//CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;			 //BTR-SJW ����ͬ����Ծ��� 1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;			 //BTR-TS1 ʱ���1 ռ����2��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;			 //BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_Prescaler =12;

	/*switch(500)
	{
		case 1000 : CAN_InitStructure.CAN_Prescaler =6;// 1MBps  //BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+2+3)/6=1Mbps
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

	  /* CAN��������ʼ�� */
	  CAN_FilterInitStructure.CAN_FilterNumber=0;						//
	  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//FM1R	��������0�Ĺ���ģʽ��
																		//0: ��������x��2��32λ�Ĵ��������ڱ�ʶ������λģʽ��
																		///1:��������x��2��32λ�Ĵ��������ڱ�ʶ���б�ģʽ��
	  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//FS1R ��������0(13��0)��λ��
																		//0��������λ��Ϊ2��16λ�� 1��������λ��Ϊ����32λ��

	  /* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */
	  CAN_FilterInitStructure.CAN_FilterIdHigh=(((u32)slave_id<<21)&0xffff0000)>>16;				//Ҫ���˵�ID��λ
	  CAN_FilterInitStructure.CAN_FilterIdLow=(((u32)slave_id<<21)|CAN_ID_STD)&0xffff;				//Ҫ���˵�ID��λ
	  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;;
	  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xfffC;
	  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;		//FFAx : ������λ������ ������ͨ����ĳ�������Ĺ��˺�
																		//������ŵ��������FIFO�С� 0����������������FIFO0�� 1����������������FIFO1��
	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;				//FACTx : ���������� �����ĳλ����1��������Ӧ�Ĺ�������ֻ�ж�FACTxλ��0��
																		//���CAN_FMR�Ĵ�����FINITλ����1�󣬲����޸���Ӧ�Ĺ������Ĵ���
																		//x(CAN_FxR[0:1])�� 0�������������ã� 1�������������
	  CAN_FilterInit(&CAN_FilterInitStructure);

	  /* CAN FIFO0 �����ж�ʹ�� */
	  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

}

void NRF24L01_Address_Configuration()
{
	  /*  NRF24L01��6ͨ�������ַ�ĳ�ʼ�� */
  TX_ADDRESS0[0]=0x34;	            //ͨ��0 �����ַ
  TX_ADDRESS0[1]=0x43;
  TX_ADDRESS0[2]=0x10;
  TX_ADDRESS0[3]=0x10;
  TX_ADDRESS0[4]=0x01;

  TX_ADDRESS1[0]=0x01;				//ͨ��1 �����ַ
  TX_ADDRESS1[1]=0xE1;
  TX_ADDRESS1[2]=0xE2;
  TX_ADDRESS1[3]=0xE3;
  TX_ADDRESS1[4]=0x02;

  TX_ADDRESS2[0]=0x02;			   //ͨ��2 �����ַ
  TX_ADDRESS2[1]=0xE1;
  TX_ADDRESS2[2]=0xE2;
  TX_ADDRESS2[3]=0xE3;
  TX_ADDRESS2[4]=0x02;

  TX_ADDRESS3[0]=0x03;			   //ͨ��3 �����ַ
  TX_ADDRESS3[1]=0xE1;
  TX_ADDRESS3[2]=0xE2;
  TX_ADDRESS3[3]=0xE3;
  TX_ADDRESS3[4]=0x02;

  TX_ADDRESS4[0]=0x04;			   //ͨ��4 �����ַ
  TX_ADDRESS4[1]=0xE1;
  TX_ADDRESS4[2]=0xE2;
  TX_ADDRESS4[3]=0xE3;
  TX_ADDRESS4[4]=0x02;

  TX_ADDRESS5[0]=0x05;			   //ͨ��5 �����ַ
  TX_ADDRESS5[1]=0xE1;
  TX_ADDRESS5[2]=0xE2;
  TX_ADDRESS5[3]=0xE3;
  TX_ADDRESS5[4]=0x02;
}

void ADC_Configruation(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

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
    //ѭ��ģʽ������Bufferд�����Զ��ص���ʼ��ַ��ʼ����
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    //������ɺ�����DMAͨ��
    DMA_Cmd(DMA1_Channel1, ENABLE);

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);



    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //����ת������
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 5;     //����ת�����г���Ϊ4
    ADC_Init(ADC1, &ADC_InitStructure);


    //����ת������1��ͨ��10
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 4, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 5, ADC_SampleTime_239Cycles5);

    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
    ADC_DMACmd(ADC1, ENABLE);

    // ������ADC�Զ�У׼����������ִ��һ�Σ���֤����
    // Enable ADC1 reset calibaration register
    ADC_ResetCalibration(ADC1);
    // Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));

    // Start ADC1 calibaration
    ADC_StartCalibration(ADC1);
    // Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
    // ADC�Զ�У׼����---------------
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);


}
void RTC_Configuration(void)
{
	PWR_BackupAccessCmd(ENABLE);
	if(BKP_ReadBackupRegister(BKP_Check) != 0xA5A5)
	{
	  USART_printf(USART2, "Backup lost Power!\r\n");
	  /* Enable PWR and BKP clocks */
//	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	  /* Allow access to BKP Domain */
//	  PWR_BackupAccessCmd(ENABLE);

	  /* Reset Backup Domain */
	  BKP_DeInit();

	  /* Enable LSE */
	  RCC_LSEConfig(RCC_LSE_ON);
	  /* Wait till LSE is ready */
	  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	  {}

	  /* Select LSE as RTC Clock Source */
	  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	  /* Enable RTC Clock */
	  RCC_RTCCLKCmd(ENABLE);

	  /* Wait for RTC registers synchronization */
	  RTC_WaitForSynchro();

	  /* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();

	  /* Enable the RTC Second */
	  RTC_ITConfig(RTC_IT_SEC, ENABLE);

	  /* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();

	  /* Set RTC prescaler: set RTC period to 1sec */
	  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

	  /* Wait until last write operation on RTC registers has finished */
//	  RTC_WaitForLastTask();
//	  Time_Adjust();
	  BKP_WriteBackupRegister(BKP_Check, 0xA5A5);
	}
	else
	{
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */

	}
	Online_Time=BKP_ReadBackupRegister(BKP_Run_time);
}

//�رյ��Խӿڣ���GPIOʹ��
void UnableJTAG(void)
{

   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

}


////SYSTICK ���ã� 1msһ��systick�жϣ�����ucos ���Ƚ��ģ� 1KHz
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

