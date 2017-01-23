/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
INT8U SD_TIMER= 0;
void SysTick_Handler(void)
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntNesting++;
	OS_EXIT_CRITICAL();

	OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */
	SD_TIMER++;
	if(SD_TIMER>=10)
	{
		disk_timerproc();
		SD_TIMER==0;
	}

	OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


void RTC_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntNesting++;
	OS_EXIT_CRITICAL();
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Reset RTC Counter when Time is 23:59:59 */
    if (RTC_GetCounter() == 0x00015180)
    {
      RTC_SetCounter(0x0);
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
    }
	//GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))));
  }
  BKP_WriteBackupRegister(BKP_Run_time, Online_Time++);


  OSIntExit();			   //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();						 /* Tell uC/OS-II that we are starting an ISR		   */
	OSIntNesting++;
	OS_EXIT_CRITICAL();

	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{

		CAN_Receive(CAN1,CAN_FIFO0,&can_rx_msg);
		OSSemPost(sem_can_rx);
                CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);

	}


	OSIntExit();								 /* Tell uC/OS-II that we are leaving the ISR		   */
}

void EXTI0_IRQHandler(void)
{
	unsigned char status;
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();	//保存全局中断标志,关总中断 Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;
	OS_EXIT_CRITICAL();	  //恢复全局中断标志


	if(EXTI_GetITStatus(EXTI_Line0) != RESET) 		//判断是否产生了EXTI0中断
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)//判断是否是PA0线变低
		{
			status=SPI_Read(READ_REG1+STATUS);			// 读取状态寄存其来判断数据接收状况
			if(status & 0x40)							// 判断是否接收到数据
			{
				SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);  //从接收缓冲区里读出数据
				if((status&0x0e)<=0x0a)
				{
					nrf_Pipe_r=(status&0x0e)>>1; 				  //读出是在哪个通道接收的
				}
				else nrf_Pipe_r=0;
		//		Rx_Succ=1;											//读取数据完成标志
				/* 根据读出的接收通道号，将相应信息写入状态文本缓冲区 */
		//		if(nrf_Pipe_r==0) memcpy(status_buf, "Pipe 0 Recive OK!   ", 20);
		//		else if(nrf_Pipe_r==1) memcpy(status_buf, "Pipe 1 Recive OK!   ", 20);
		//		else if(nrf_Pipe_r==2) memcpy(status_buf, "Pipe 2 Recive OK!   ", 20);
		//		else if(nrf_Pipe_r==3) memcpy(status_buf, "Pipe 3 Recive OK!   ", 20);
		//		else if(nrf_Pipe_r==4) memcpy(status_buf, "Pipe 4 Recive OK!   ", 20);
		//		else if(nrf_Pipe_r==5) memcpy(status_buf, "Pipe 5 Recive OK!   ", 20);
	/*		}
			else if((status &0x10)>0)					//发射达到最大复发次数
			{
				SPI_RW_Reg(0xe1,0); 					 //清除发送缓冲区
				RX_Mode();								 //进入接收模式
				Rx_Succ=1;
				/* 根据发送通道，将相应信息写入状态文本缓冲区 */
	/*			if(nrf_Pipe==0) memcpy(status_buf, "Pipe 0 NO ACK!		", 20);
				else if(nrf_Pipe==1) memcpy(status_buf, "Pipe 1 NO ACK! 	 ", 20);
				else if(nrf_Pipe==2) memcpy(status_buf, "Pipe 2 NO ACK! 	 ", 20);
				else if(nrf_Pipe==3) memcpy(status_buf, "Pipe 3 NO ACK! 	 ", 20);
				else if(nrf_Pipe==4) memcpy(status_buf, "Pipe 4 NO ACK! 	 ", 20);
				else if(nrf_Pipe==5) memcpy(status_buf, "Pipe 5 NO ACK! 	 ", 20);
			}
			else if((status &0x20)>0)					//发射后收到应答
			{
				SPI_RW_Reg(0xe1,0); 						//清除发送缓冲区
				RX_Mode();								 	//进入接收模式
				Rx_Succ=1;
				/* 根据发送通道，将相应信息写入状态文本缓冲区 */
/*				if(nrf_Pipe==0) memcpy(status_buf, "Pipe 0 Send OK! 	", 20);
				else if(nrf_Pipe==1) memcpy(status_buf, "Pipe 1 Send OK!	 ", 20);
				else if(nrf_Pipe==2) memcpy(status_buf, "Pipe 2 Send OK!	 ", 20);
				else if(nrf_Pipe==3) memcpy(status_buf, "Pipe 3 Send OK!	 ", 20);
				else if(nrf_Pipe==4) memcpy(status_buf, "Pipe 4 Send OK!	 ", 20);
				else if(nrf_Pipe==5) memcpy(status_buf, "Pipe 5 Send OK!	 ", 20);*/
			}

			SPI_RW_Reg(WRITE_REG1+STATUS, status);		 //清除07寄存器标志
	  }
	  EXTI_ClearITPendingBit(EXTI_Line0);			 //清除EXTI0上的中断标志
	}
	OSIntExit();			 //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

extern void GPRMC_DAT(void);

void USART1_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntNesting++;
	OS_EXIT_CRITICAL();

	static vu8 RxCounter1;
	unsigned int i;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		GPSRxBuffer[RxCounter1++] = USART_ReceiveData(USART1);   //将读寄存器的数据缓存到接收缓冲区里
    	if(GPSRxBuffer[RxCounter1-1]=='$'){GPSRxBuffer[0]='$'; RxCounter1=1;}     //判断起始标志
		if(GPSRxBuffer[RxCounter1-1]==0x0a)                      //判断结束标志是否是0x0d 0x0a
		{
			for(i=0; i< RxCounter1; i++) GPSTxBuffer[i]	= GPSRxBuffer[i]; 		 //将接收缓冲器的数据转到发送缓冲区，准备转发															 //接收成功标志
			Rec_Len=RxCounter1;
			RxCounter1=0;

			if(GPSTxBuffer[3]=='R'&& GPSTxBuffer[4]=='M'&&GPSTxBuffer[5]=='C')
			{
				OSSemPost(sem_GPS);			//接收完成，传送信号量

			}


    	}
	}
	OSIntExit();			 //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换
}

void DMA1_Channel1_IRQHandler(void)
{
		OS_CPU_SR  cpu_sr;
		INT8U i = 0,j=0;
		INT32U sum;

		OS_ENTER_CRITICAL();						 /* Tell uC/OS-II that we are starting an ISR		   */
		OSIntNesting++;
		OS_EXIT_CRITICAL();
		if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
		{
			for(i=0;i<ADC_N;i++)
			{
				sum=0;
				for(j=0;j<ADC_M;j++)
				{
					sum+=AD_Value[j][i];
				}
				AD_Value_Arg[i] = sum/ADC_M;

			}

			DMA_ClearITPendingBit(DMA1_IT_TC1);
		}

		OSIntExit();								 /* Tell uC/OS-II that we are leaving the ISR		   */
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
