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

#include "globals.h"

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

/*
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntNesting++;
	OS_EXIT_CRITICAL();

	OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */

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

	OSIntExit();
							 /* Tell uC/OS-II that we are leaving the ISR		   */
}

void USART1_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	unsigned int i;
        unsigned char msg[50];
	OS_ENTER_CRITICAL();  //±£´æÈ«¾ÖÖÐ¶Ï±êÖ¾,¹Ø×ÜÖÐ¶Ï// Tell uC/OS-II that we are starting an ISR
  	OSIntNesting++;
  	OS_EXIT_CRITICAL();	  //»Ö¸´È«¾ÖÖÐ¶Ï±êÖ¾

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	   //ÅÐ¶Ï¶Á¼Ä´æÆ÷ÊÇ·ñ·Ç¿Õ
  	{
		 //    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    		// Read one byte from the receive data register

			msg[RxCounter1++]= USART_ReceiveData(USART1);   //½«¶Á¼Ä´æÆ÷µÄÊý¾Ý»º´æµ½½ÓÊÕ»º³åÇøÀï
			if(msg[RxCounter1-1] == '\n')					//ÅÐ¶ÏÊÇ·ñ½áÊøå
			{
				RxCounter1=0;
				OSMboxPost(Mbox_Com1,(void *)&msg);
			}
  	}
  	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                     //
  	{
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  	}


  	OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}
void EXTI9_5_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;

	INT8U os_err=0;
	OS_FLAGS Flags;

	OS_ENTER_CRITICAL();  //±£´æÈ«¾ÖÖÐ¶Ï±êÖ¾,¹Ø×ÜÖÐ¶Ï// Tell uC/OS-II that we are starting an ISR
  	OSIntNesting++;
  	OS_EXIT_CRITICAL();	  //»Ö¸´È«¾ÖÖÐ¶Ï±êÖ¾

	Flags = OSFlagQuery(Sem_F, &os_err);

	if(EXTI_GetITStatus(EXTI_Line9) != RESET)                              	//IN3  Net_IN
	{
		#ifdef DEBUG
		USART_printf(USART1, "\r\n Netural_IN Interrupt! \r\n");
		#endif

		EXTI_ClearITPendingBit(EXTI_Line9);

		OSSemPost(Sem_neu);


	}
	else
	{
		if(Flags != 0)
		{
			#ifdef DEBUG
			USART_printf(USART1, "\r\n Error ! Task Running! \r\n");
			#endif
			EXTI -> PR |= 0x01C0;					//Çå³ýIN1~3ÖÐ¶ÏÎ»																//»»µ²ÖÐ ÎÞÊÓÐÅºÅ
		}

		else if(EXTI_GetITStatus(EXTI_Line7) != RESET)                  //IN1  UP_IN
		{

                         #ifdef DEBUG
                         USART_printf(USART1, "\r\n UP_IN Interrupt! \r\n");
                         #endif

                         EXTI_ClearITPendingBit(EXTI_Line7);

                         OSFlagPost(Sem_F, FILTER_UP, OS_FLAG_SET, &os_err);


		}

		else if(EXTI_GetITStatus(EXTI_Line8) != RESET)                               	//IN2  Down_IN
		{

                         #ifdef DEBUG
                         USART_printf(USART1, "\r\n DOWN_IN Interrupt! \r\n");
                         #endif

                         EXTI_ClearITPendingBit(EXTI_Line8);

                         OSFlagPost(Sem_F, FILTER_DOWN, OS_FLAG_SET, &os_err);

		}
		else
		{
			#ifdef DEBUG
			USART_printf(USART1, "\r\n Wrong Interrupt! In signal\r\n");
			#endif
			while(1);
		}

	}

	OSIntExit();  //ÔÚos_core.cÎÄ¼þÀï¶¨Òå,Èç¹ûÓÐ¸ü¸ßÓÅÏÈ¼¶µÄÈÎÎñ¾ÍÐ÷ÁË,ÔòÖ´ÐÐÒ»´ÎÈÎÎñÇÐ»»
}

/**
  * @brief  This function handles EXTI15_10(KEY) interrupt request.
  * @param  None
  * @retval None
  */

void EXTI15_10_IRQHandler(void)
{

	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();  //±£´æÈ«¾ÖÖÐ¶Ï±êÖ¾,¹Ø×ÜÖÐ¶Ï// Tell uC/OS-II that we are starting an ISR
  	OSIntNesting++;
  	OS_EXIT_CRITICAL();	  //»Ö¸´È«¾ÖÖÐ¶Ï±êÖ¾

	if(EXTI_GetITStatus(EXTI_Line11) != RESET)										//KEY1
	{
		#ifdef DEBUG
		USART_printf(USART1, "\r\n KEY Interrupt! \r\n");
		#endif
		EXTI_GenerateSWInterrupt(EXTI_Line8);
		EXTI_ClearITPendingBit(EXTI_Line11);



	}
	else
	{
		#ifdef DEBUG
		USART_printf(USART1, "\r\n Wrong Interrupt! In key \r\n");
		#endif

	}


    OSIntExit();  //ÔÚos_core.cÎÄ¼þÀï¶¨Òå,Èç¹ûÓÐ¸ü¸ßÓÅÏÈ¼¶µÄÈÎÎñ¾ÍÐ÷ÁË,ÔòÖ´ÐÐÒ»´ÎÈÎÎñÇÐ»»

}
/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
