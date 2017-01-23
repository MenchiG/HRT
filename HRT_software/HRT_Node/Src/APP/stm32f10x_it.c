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
	//	CAN_FIFORelease(CAN1, CAN_FIFO0 );
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	}


	OSIntExit();								 /* Tell uC/OS-II that we are leaving the ISR		   */
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


void TIM4_IRQHandler(void)
{
                                             //频率缓冲区计数
	static u16 this_time_CH1 = 0;
	static u16 last_time_CH1 = 0;
	static u8 capture_number_CH1 = 0;
	vu16 tmp16_CH1;

	static u16 this_time_CH2 = 0;
	static u16 last_time_CH2 = 0;
	static u8 capture_number_CH2 = 0;
	vu16 tmp16_CH2;

	static u16 this_time_CH3 = 0;
	static u16 last_time_CH3 = 0;
	static u8 capture_number_CH3 = 0;
	vu16 tmp16_CH3;

	static u16 this_time_CH4 = 0;
	static u16 last_time_CH4 = 0;
	static u8 capture_number_CH4 = 0;
	vu16 tmp16_CH4;


  if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);

        if(capture_number_CH1 == 0)
        {
            capture_number_CH1 = 1;
            last_time_CH1 = TIM_GetCapture1(TIM4);
        }
        else if(capture_number_CH1 == 1)
        {
            capture_number_CH1 = 0;
            this_time_CH1 = TIM_GetCapture1(TIM4);

            if(this_time_CH1 > last_time_CH1)
            {
                tmp16_CH1 = (this_time_CH1 - last_time_CH1);
            }
            else
            {
                tmp16_CH1 = ((0xFFFF - last_time_CH1) + this_time_CH1);
            }

			Freq_Value[0]=tmp16_CH1;
        }
  }

    if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);

        if(capture_number_CH2 == 0)
        {
            capture_number_CH2 = 1;
            last_time_CH2 = TIM_GetCapture2(TIM4);
        }
        else if(capture_number_CH2 == 1)
        {
            capture_number_CH2 = 0;
            this_time_CH2 = TIM_GetCapture2(TIM4);

            if(this_time_CH2 > last_time_CH2)
            {
                tmp16_CH2 = (this_time_CH2 - last_time_CH2);
            }
            else
            {
                tmp16_CH2 = ((0xFFFF - last_time_CH2) + this_time_CH2);
            }

			Freq_Value[1]=tmp16_CH2;
        }
	}
}


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

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
