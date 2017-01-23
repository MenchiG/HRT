/**************************************************
uCosII STM32平台
例程工程
创建日期：2012-12-1
版本：V1.0	创建工程文件
**************************************************/



/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define GLOBALS
#include "includes.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static	OS_STK TaskStartStk[TASK_START_STK_SIZE];
static	OS_STK TaskADCStk[TASK_ADC_STK_SIZE];
static	OS_STK TaskCANRxStk[TASK_CAN_RX_STK_SIZE];

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void App_TaskCreate(void);

u16 GetVolt(u16 advalue);


static void TaskADC(void *p_arg);
static void TaskStart(void *p_arg);
static void TaskCanRx(void *p_arg);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument : none.
*
* Return   : none.
*********************************************************************************************************
*/
int main()
{
	INT8U os_err=0;

//	CPU_IntDis();

	OSInit();

	BSP_Init();

	os_err = OSTaskCreate((void (*) (void *))TaskStart,	  		  	//指向任务代码的指针
                          (void *) 0,								  		//任务开始执行时，传递给任务的参数的指针
               	(OS_STK *) &TaskStartStk[TASK_START_STK_SIZE- 1],	//分配给任务的堆栈的栈顶指针   从顶向下递减
               (INT8U) TASK_START_PRIO);								//分配给任务的优先级
	OSTaskNameSet(TASK_START_PRIO, "TaskStart", &os_err);

//OSTimeSet(0);
	OSStart();

	return(0);
}

static void TaskStart(void *p_arg)
{
	(void)p_arg;


	OS_CPU_SysTickInit();

	#if (OS_TASK_STAT_EN > 0)
	OSStatInit();
	#endif



	sem_can_rx = OSSemCreate(0);
	sem_adc = OSSemCreate(0);

	App_TaskCreate();

	while(1)
	{
		LED_ON();


		OSTimeDlyHMSM(0, 0, 1, 0);

		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
static void TaskCanRx(void *p_arg)
{
	INT8U os_err=0;
	(void)p_arg;
	CAN_ON();
	do
	{
		OSSemPend(sem_can_rx, 0, &os_err);
	}while(can_rx_msg.Data[0] != CAN_SIG);
	can_tx_msg.StdId = master_id;
	can_tx_msg.RTR = CAN_RTR_REMOTE;		//远程帧
	can_tx_msg.IDE  = CAN_ID_STD;
	LED_OFF();
	CAN_Transmit(CAN1, &can_tx_msg);
	while(1)
	{
		OSSemPend(sem_can_rx, 0, &os_err);

		can_tx_msg.StdId = 0x070;
		can_tx_msg.DLC = 8;
		can_tx_msg.RTR = CAN_RTR_DATA;		//0数据帧
		can_tx_msg.IDE  = CAN_ID_STD;	//0标准帧
		can_tx_msg.Data[0] = CAN_STR;
		can_tx_msg.Data[1] = 0x02;
		can_tx_msg.Data[2] = AD_Value_Arg[0]>>8;
		can_tx_msg.Data[3] = AD_Value_Arg[0];
		can_tx_msg.Data[4] = AD_Value_Arg[1]>>8;
		can_tx_msg.Data[5] = AD_Value_Arg[1];
		can_tx_msg.Data[6] = AD_Value_Arg[2]>>8;
		can_tx_msg.Data[7] = AD_Value_Arg[2];
//		OSTimeDlyHMSM(0, 0, 0, 0);
		CAN_Transmit(CAN1, &can_tx_msg);
		OSSemPend(sem_can_rx, 0, &os_err);

		can_tx_msg.StdId = 0x070;
		can_tx_msg.DLC = 8;
		can_tx_msg.RTR = CAN_RTR_DATA;		//0数据帧
		can_tx_msg.IDE  = CAN_ID_STD;	//0标准帧
		can_tx_msg.Data[0] = CAN_END | 0x01;
		can_tx_msg.Data[1] = 0x02;
		can_tx_msg.Data[2] = AD_Value_Arg[3]>>8;
		can_tx_msg.Data[3] = AD_Value_Arg[3];
		can_tx_msg.Data[4] = Freq_Value[0]>>8;
		can_tx_msg.Data[5] = Freq_Value[0];
		can_tx_msg.Data[6] = Freq_Value[1]>>8;
		can_tx_msg.Data[7] = Freq_Value[1];

		CAN_Transmit(CAN1, &can_tx_msg);
	//CAN_OFF();

	}
}

static void TaskADC(void *p_arg)
{
	INT8U os_err=0;

	INT16U *msg;
	s16  Temp;
	(void)p_arg;
	while(1)
	{
			OSSemPend(sem_adc, 0, &os_err);


				USART_printf(USART1,"%d\r\n",AD_Value_Arg[0]*330/4096);
				USART_printf(USART1,"%d\r\n",AD_Value_Arg[1]*330/4096);
				USART_printf(USART1,"%d\r\n",AD_Value_Arg[2]*330/4096);
				USART_printf(USART1,"%d\r\n",AD_Value_Arg[3]*330/4096);


			    USART_printf(USART1,"%d\r\n",Freq_Value[0]);
		        USART_printf(USART1,"%d\r\n",Freq_Value[1]);
				OSTimeDlyHMSM(0, 0, 1, 0);

	}

}

static void App_TaskCreate()
{
	INT8U os_err=0;
	os_err =OSTaskCreate((void (*) (void *))TaskCanRx,
						(void *)0,
						(OS_STK *) &TaskCANRxStk[TASK_CAN_RX_STK_SIZE-1],
						TASK_CAN_RX_PRIO);
	OSTaskNameSet(TASK_CAN_RX_PRIO, "TaskCanRx", &os_err);
	os_err =OSTaskCreate((void (*) (void *))TaskADC,
						(void *)0,
						(OS_STK *) &TaskADCStk[TASK_ADC_STK_SIZE- 1],
						TASK_ADC_PRIO);
	OSTaskNameSet(TASK_ADC_PRIO, "TaskADC", &os_err);
}

u16 GetVolt(u16 advalue)
{
    return (u16)((advalue * 330 / 4096)*156/100);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument : ptcb   is a pointer to the task control block of the task being created.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskCreateHook(OS_TCB* ptcb)
{
}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskDelHook(OS_TCB* ptcb)
{
   (void) ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument : none.
*
* Note     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void App_TaskIdleHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument : none.
*********************************************************************************************************
*/

void App_TaskStatHook(void)
{
}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument : none.
*
* Note     : 1 Interrupts are disabled during this call.
*
*            2  It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void App_TaskSwHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument : ptcb    is a pointer to the TCB of the task being created.
*
* Note     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void App_TCBInitHook(OS_TCB* ptcb)
{
   (void) ptcb;
}
#endif

void App_TimeTickHook     (void)
{
	IWDG_ReloadCounter();
}

#endif

