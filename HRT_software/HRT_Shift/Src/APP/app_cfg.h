/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-LK1 Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.10
* Programmer(s) : BH3NVN
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/
#define  TASK_START_PRIO 								2u

#define  TASK_CAN_PRIO									3u
#define  TASK_COM1_PRIO									4u


#define  TASK_NEUTRAL_PRIO                         		5u
#define  TASK_UP_PRIO                       			6u
#define  TASK_DOWN_PRIO                         		7u



#define  OS_TASK_TMR_PRIO								(OS_LOWEST_PRIO-2u)

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define	TASK_START_STK_SIZE                     	128u

#define	TASK_CAN_STK_SIZE							128u
#define TASK_COM1_STK_SIZE 							128u

#define	TASK_UP_STK_SIZE                         	  128u
#define	TASK_DOWN_STK_SIZE                         	  128u
#define	TASK_NEUTRAL_STK_SIZE						  128u



#endif
