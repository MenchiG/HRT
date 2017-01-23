/**************************************************
uCosII STM32平台
例程工程
创建日期：2012-12-1
版本：V1.0	创建工程文件
已完成:
信号输入输出 串口输入设置
未完成:
ADC可调
CAN总线相关

**************************************************/



/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "includes.h"


#define GLOBALS
#include "globals.h"




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
static	OS_STK TaskUpStk[TASK_UP_STK_SIZE];
static	OS_STK TaskDownStk[TASK_DOWN_STK_SIZE];
static	OS_STK TaskNeutralStk[TASK_NEUTRAL_STK_SIZE];
static	OS_STK TaskCANStk[TASK_CAN_STK_SIZE];
static	OS_STK TaskCom1Stk[TASK_COM1_STK_SIZE];

uint16_t VirtAddVarTab[NumbOfVar] = {0x5555, 0x6666, 0x7777};


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void App_TaskCreate(void);

static void TaskStart(void *p_arg);
static void TaskUp(void *p_arg);
static void TaskDown(void *p_arg);
static void TaskReturn(void *p_arg);
static void TaskCom1(void *p_arg);

INT16S atoi(INT8U str[]);

void USART_Time_Table(void);

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



	os_err = OSTaskCreate((void (*) (void *))  TaskStart,	  		  	//指向任务代码的指针
                          (void *) 0,								  	//任务开始执行时，传递给任务的参数的指针
               	(OS_STK *) &TaskStartStk[TASK_START_STK_SIZE - 1],		//分配给任务的堆栈的栈顶指针   从顶向下递减
               (INT8U) TASK_START_PRIO);								//分配给任务的优先级
	OSTaskNameSet(TASK_START_PRIO, "TaskStart", &os_err);


//	OSTimeSet(0)
	OSStart();

	return(0);
}

static void TaskStart(void *p_arg)
{
	INT8U i;
	INT8U os_err=0;
	(void)p_arg;


	OS_CPU_SysTickInit();

	#if (OS_TASK_STAT_EN > 0)
	OSStatInit();
	#endif


        BSP_Init();
	//	0~6  dly up1~3 down 1~3

	s16_time[UPT1] = 20;	 //20
	s16_time[UPT2] = 200;	 //30~150
	s16_time[UPT3] = 20;	 //20

	s16_time[DOT1] = 30;	//	0 50
	s16_time[DOT2] = 150;	//	30 150
	s16_time[DOT3] = 30;	//	0 50
	s16_time[DLY] = 100;
	s16_time[NEU] = 50 ;

	control_counter_UP = 0;
        control_counter_DOWN = 0;
	RPM = 0;

	for(i=0;i<8;i++)
	{
		os_err = EE_ReadVariable(VirtAddVarTab[0]+i, &s16_time[i]);
	}
	if(os_err != 0)
	{
		USART_printf(USART1,"\r\nError %d read data faild!!!\r\n",os_err);
	}
	else
	{
		USART_printf(USART1,"\r\nread data succeed!!!\r\n");
	}

	Sem_F = OSFlagCreate(0, &os_err);					//创建信号量集
	OSFlagNameSet(Sem_F, "Sem_F", &os_err);

	Mbox_Com1 = OSMboxCreate((void *) 0);
	OSEventNameSet(Mbox_Com1, "Mbox_Com1", &os_err);

	sem_can_rx = OSSemCreate(0);
	OSEventNameSet(sem_can_rx, "Sem_CAN", &os_err);

	Sem_neu = OSSemCreate(0);
	OSEventNameSet(Sem_neu, "Sem_neu", &os_err);

	App_TaskCreate();


	USART_printf(USART1, "\r\nApp create...succeed \r\n");
	USART_Time_Table();

	while(1)
	{


	LED_LED1_ON();
	OSTimeDlyHMSM(0,0,1,0);
	/*	can_tx_msg.StdId = 0xfff;
		can_tx_msg.DLC = 8;
		can_tx_msg.RTR = CAN_RTR_DATA;		//0数据帧
		can_tx_msg.IDE  = CAN_ID_STD;	//0标准帧
		can_tx_msg.Data[0] = 0;
		can_tx_msg.Data[1] = 0x01;
		can_tx_msg.Data[2] = 0;
		can_tx_msg.Data[3] = 0;
		can_tx_msg.Data[4] = 0;
		can_tx_msg.Data[5] = 0;
		can_tx_msg.Data[6] = 0;
		can_tx_msg.Data[7] = 0;
        CAN_Transmit(CAN1, &can_tx_msg);*/
	LED_LED1_OFF();
	OSTimeDlyHMSM(0,0,1,0);

	}
}

static void TaskUp(void *p_arg)
{

	INT8U os_err=0;

	(void)p_arg;

	while (1)
	{
		OSFlagPend(Sem_F, FILTER_UP, OS_FLAG_WAIT_SET_ALL, 0, &os_err);


		#ifdef DEBUG
		USART_printf(USART1 ,"\r\nRun Task Up!\r\nUp Times:  %d\r\n",++control_counter_UP);
		#endif

		if(RPM < 100 ||RPM > 500)
		{
			LED_LED1_ON();
			SIGNAL_UP_ON();
			OSTimeDlyHMSM(0, 0, 0, s16_time[UPT2]);

			if(OSSemAccept(Sem_neu) != 0)
			{

			#ifdef DEBUG
				USART_printf(USART1 ,"\r\nNeutral!\r\n");
			#endif

				OSTimeDlyHMSM(0, 0, 0, s16_time[NEU]);
			}

			SIGNAL_UP_OFF();


			LED_LED1_OFF();
		}
		else
		{
			USART_printf(USART1 ,"\r\nRPM Oups~\r\n");
		}
		OSFlagPost(Sem_F, FILTER_UP, OS_FLAG_CLR, &os_err);


	}
}

static void TaskDown(void *p_arg)
{
	INT8U os_err=0;

	(void)p_arg;

	while (1)
	{

		OSFlagPend(Sem_F, FILTER_DOWN, OS_FLAG_WAIT_SET_ALL, 0, &os_err);
		if(RPM < 100 ||RPM > 500)
		{


			#ifdef DEBUG
			USART_printf(USART1 ,"\r\nRun Task Down!\r\nDown Times: %d\r\n",++control_counter_DOWN);
			#endif


			LED_LED2_ON();
			SIGNAL_Clu_ON();
			OSTimeDlyHMSM(0, 0, 0, s16_time[DOT1]);
			SIGNAL_DOWN_ON();
			OSTimeDlyHMSM(0, 0, 0, s16_time[DOT2]);

			if(OSSemAccept(Sem_neu) != 0)
			{

			#ifdef DEBUG
				USART_printf(USART1 ,"\r\nNeutral!\r\n");
			#endif

				OSTimeDlyHMSM(0, 0, 0, s16_time[NEU]);
			}
			SIGNAL_DOWN_OFF();
			OSTimeDlyHMSM(0, 0, 0, s16_time[DOT3]);
			SIGNAL_Clu_OFF();
			OSTimeDlyHMSM(0, 0, 0, s16_time[DLY]);

			LED_LED2_OFF();
		}
		else
		{
			USART_printf(USART1 ,"\r\nRPM Oups~\r\n");
		}

		OSFlagPost(Sem_F, FILTER_DOWN, OS_FLAG_CLR, &os_err);
	}
}

static void TaskNeutral(void *p_arg)
{
	INT8U os_err=0;

	(void)p_arg;

	while (1)
	{
		//OSFlagPend(Sem_F, FILTER_RETURN, OS_FLAG_WAIT_SET_ALL, 0, &os_err);
		OSTimeDlyHMSM(1, 0, 0, 0);

/*		OSSemPend(Sem_neu, 0, &os_err);

		#ifdef DEBUG
		USART_printf(USART1 ,"\r\nNeutral!\r\n");
		#endif

		OSTimeDlyHMSM(0, 0, 0, s16_time[NEU]);*/

		//	OSTimeDlyHMSM(0, 0, 0, s16_time[DLY]);

		//OSFlagPost(Sem_F, FILTER_RETURN, OS_FLAG_CLR, &os_err);
	}
}

static void TaskCom1(void *p_arg)
{
	INT8U os_err = 0;
	INT8U *msg;
	INT8U i;
	(void)p_arg;
	FLASH_Status FLASH_FLAG;
	while(1)
	{
		msg = (INT8U *)OSMboxPend(Mbox_Com1, 0, &os_err);
		if(msg[0] == 'T')
		{
			switch(msg[1])
			{
				case '0':s16_time[DLY] = atoi(&msg[2]);
						USART_printf(USART1,"\r\nTimeDelay has been set to %d ms!\r\n",s16_time[DLY]);
						break;
				case '1':s16_time[UPT1] = atoi(&msg[2]);
						USART_printf(USART1,"\r\nAUTO_Up_RPM has been set to %d RPM!\r\n",s16_time[UPT1]);
						break;
				case '2':s16_time[UPT2] = atoi(&msg[2]);
						USART_printf(USART1,"\r\nTimeUpT2 has been set to %d ms!\r\n",s16_time[UPT2]);
						break;
				case '3':s16_time[UPT3] = atoi(&msg[2]);
						USART_printf(USART1,"\r\nTimeUpT3 has been set to %d ms!\r\n",s16_time[UPT3]);
						break;
				case '4':s16_time[DOT1] = atoi(&msg[2]);
						USART_printf(USART1,"\r\nTimeDownT1 has been set to %d ms!\r\n",s16_time[DOT1]);
						break;
				case '5':s16_time[DOT2] = atoi(&msg[2]);
						USART_printf(USART1,"\r\nTimeDownT2 has been set to %d ms!\r\n",s16_time[DOT2]);
						break;
				case '6':s16_time[DOT3] = atoi(&msg[2]);
						USART_printf(USART1,"\r\nTimeDownT3 has been set to %d ms!\r\n",s16_time[DOT3]);
						break;
				case '7':s16_time[NEU] = atoi(&msg[2]);
						USART_printf(USART1,"\r\nTimeNeutral has been set to %d ms!\r\n",s16_time[NEU]);
						break;


				case 'S':
						for(i=0;i<8;i++)
						{
							FLASH_FLAG = EE_WriteVariable(VirtAddVarTab[0]+i, s16_time[i]);
						}
						if(FLASH_FLAG != FLASH_COMPLETE)
						{
							USART_printf(USART1,"\r\nError %d save data faild!!!\r\n",FLASH_FLAG);
						}
						else
						{
							USART_printf(USART1,"\r\nsave data succeed!!!\r\n");
						}
						break;
                                case 'T':       break;             
				default:
						USART_printf(USART1,"\r\nWrong Time Set!!!\r\n");
			}

			USART_Time_Table();
		}
		else
		{
			USART_printf(USART1,"\r\nWrong Command!!!\r\n");
		}
	}
}
static void TaskCAN(void *p_arg)
{
	INT8U os_err = 0;
	INT8U *msg;

	(void)p_arg;

	while(1)
	{

                OSSemPend(sem_can_rx, 0, &os_err);
                if(can_rx_msg.Data[0] == 0x82 && can_rx_msg.Data[1] == 0x81 && can_rx_msg.Data[2] == 0x80)
                {
					RPM = can_rx_msg.Data[4]<<8 | can_rx_msg.Data[5];
					if(uMode == MODE_ACCEL && RPM > s16_time[UPT1]*100)
					{
						USART_printf(USART1,"\r\nRPM>%d Auto Up\r\n",s16_time[UPT1]*100);
						EXTI_GenerateSWInterrupt(EXTI_Line7);
					}
                }
	}
}


static void App_TaskCreate()
{
	INT8U os_err = 0;
	os_err = OSTaskCreate(
			TaskUp,
			(void *)0,
			&TaskUpStk[TASK_UP_STK_SIZE-1],
			TASK_UP_PRIO);
	OSTaskNameSet(TASK_UP_PRIO, "TaskUp", &os_err);

	os_err = OSTaskCreate(
		TaskCAN,
		(void *)0,
		&TaskCANStk[TASK_CAN_STK_SIZE-1],
		TASK_CAN_PRIO);
	OSTaskNameSet(TASK_CAN_PRIO, "TaskCAN", &os_err);

	os_err = OSTaskCreate(
		TaskCom1,
		(void *)0,
		&TaskCom1Stk[TASK_COM1_STK_SIZE-1],
		TASK_COM1_PRIO);
	OSTaskNameSet(TASK_COM1_PRIO, "TaskCom1", &os_err);

	os_err = OSTaskCreate(
			TaskDown,
			(void *)0,
			&TaskDownStk[TASK_DOWN_STK_SIZE-1],
			TASK_DOWN_PRIO);
	OSTaskNameSet(TASK_DOWN_PRIO, "TaskDown", &os_err);

	os_err = OSTaskCreate(
			TaskNeutral,
			(void *)0,
			&TaskNeutralStk[TASK_NEUTRAL_STK_SIZE-1],
			TASK_NEUTRAL_PRIO);
	OSTaskNameSet(TASK_NEUTRAL_PRIO, "TaskNeutral", &os_err);

}

INT16S atoi(INT8U str[])
{
	INT16S uReturn;
	uReturn = ((str[0]-'0')*100) + ((str[1]-'0')*10) +(str[2]-'0');
	return uReturn;
}
 void USART_Time_Table(void)
 {

	USART_printf(USART1,"\r\n===============================================================================\r\n");
	USART_printf(USART1,"Now the setting is \r\nTimeDelay(T0)  %d\r\nTimeUp(T2)     %d\r\nTimeDownT1(T4) %d\r\nTimeDownT2(T5) %d\r\nTimeDownT3(T6) %d\r\nNeutral(T7)    %d\r\n \r\n",s16_time[DLY],s16_time[UPT2],s16_time[DOT1],s16_time[DOT2],s16_time[DOT3],s16_time[NEU]);
        USART_printf(USART1,"===============================================================================");
	switch(uMode)
	{
		case MODE_NORMAL:USART_printf(USART1, "\r\nMODE_NORMAL\r\n");
			break;
		case MODE_ACCEL:USART_printf(USART1, "\r\nMODE_ACCEL\r\n");
                                USART_printf(USART1, "\r\nAUTO_RPM_%d00(T1)\r\n",s16_time[UPT1]);
			break;
		default:USART_printf(USART1, "\r\nThis Mode is not defined!!! \r\n");
	}

 }


 /*
 *********************************************************************************************************
 *********************************************************************************************************
 *											uC/OS-II APP HOOKS
 *********************************************************************************************************
 *********************************************************************************************************
 */

#if (OS_APP_HOOKS_EN > 0)
 /*
 *********************************************************************************************************
 *										TASK CREATION HOOK (APPLICATION)
 *
 * Description : This function is called when a task is created.
 *
 * Argument : ptcb	 is a pointer to the task control block of the task being created.
 *
 * Note 	: (1) Interrupts are disabled during this call.
 *********************************************************************************************************
 */

 void App_TaskCreateHook(OS_TCB* ptcb)
 {
 }

 /*
 *********************************************************************************************************
 *									  TASK DELETION HOOK (APPLICATION)
 *
 * Description : This function is called when a task is deleted.
 *
 * Argument : ptcb	 is a pointer to the task control block of the task being deleted.
 *
 * Note 	: (1) Interrupts are disabled during this call.
 *********************************************************************************************************
 */

 void App_TaskDelHook(OS_TCB* ptcb)
 {
	(void) ptcb;
 }

 /*
 *********************************************************************************************************
 *										IDLE TASK HOOK (APPLICATION)
 *
 * Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
 *				 has been added to allow you to do such things as STOP the CPU to conserve power.
 *
 * Argument : none.
 *
 * Note 	: (1) Interrupts are enabled during this call.
 *********************************************************************************************************
 */

#if OS_VERSION >= 251
 void App_TaskIdleHook(void)
 {
 }
#endif

 /*
 *********************************************************************************************************
 *										  STATISTIC TASK HOOK (APPLICATION)
 *
 * Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
 *				 statistics task.  This allows your application to add functionality to the statistics task.
 *
 * Argument : none.
 *********************************************************************************************************
 */

 void App_TaskStatHook(void)
 {
 }

 /*
 *********************************************************************************************************
 *										  TASK SWITCH HOOK (APPLICATION)
 *
 * Description : This function is called when a task switch is performed.  This allows you to perform other
 *				 operations during a context switch.
 *
 * Argument : none.
 *
 * Note 	: 1 Interrupts are disabled during this call.
 *
 *			  2  It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
 *					 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
 *					task being switched out (i.e. the preempted task).
 *********************************************************************************************************
 */

#if OS_TASK_SW_HOOK_EN > 0
 void App_TaskSwHook(void)
 {
 }
#endif

 /*
 *********************************************************************************************************
 *									   OS_TCBInit() HOOK (APPLICATION)
 *
 * Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
 *				 up most of the TCB.
 *
 * Argument : ptcb	  is a pointer to the TCB of the task being created.
 *
 * Note 	: (1) Interrupts may or may not be ENABLED during this call.
 *********************************************************************************************************
 */

#if OS_VERSION >= 204
 void App_TCBInitHook(OS_TCB* ptcb)
 {
	(void) ptcb;
 }
#endif

 void App_TimeTickHook	   (void)
 {
	// IWDG_ReloadCounter();
 }

#endif

