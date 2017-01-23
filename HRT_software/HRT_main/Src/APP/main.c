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
#define _GLOBALS_
#include"includes.h"

//#include "app_cfg.h"
//#include "os_cfg.h"

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
OS_STK TaskStartStk[TASK_START_STK_SIZE];
OS_STK TaskSDStk[TASK_SD_STK_SIZE];
OS_STK TaskCanRxStk[TASK_CAN_RX_STK_SIZE];
OS_STK TaskGPSStk[TASK_GPS_STK_SIZE];
OS_STK TaskDataStk[TASK_GPS_STK_SIZE];


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
******************************************	***************************************************************
*/

static void App_TaskCreate(void);

static void TaskStart(void *p_arg);

static void TaskGPS(void *p_arg);
static void TaskSD(void *p_arg);

static void Task_NRF2401RX(void *p_arg);
static void Task_NRF2401TX(void *p_arg);

//static void Task_USART_RX(void *p_arg);
//static void Task_USART_TX(void *p_arg);

static void TaskMMA(void *p_arg);

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


	OSInit();



	OSTaskCreateExt((void (*)(void *)) TaskStart,
					(void			*) 0,
					(OS_STK 		*)&TaskStartStk[TASK_START_STK_SIZE - 1],
					(INT8U			 ) TASK_START_PRIO ,
					(INT16U 		 ) TASK_START_PRIO ,
					(OS_STK 		*)&TaskStartStk[0],
					(INT32U 		 ) TASK_START_STK_SIZE,
					(void			*) 0,
					(INT16U 		 )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_SIZE > 1)
			OSTaskNameSet(TASK_START_PRIO, "START", &os_err);
#endif


	OSStart();
}

static void TaskStart(void *p_arg)
{
	(void)p_arg;


	INT8U i = 0;

	OS_CPU_SysTickInit();
	BSP_Init();
#if (OS_TASK_STAT_EN > 0)
	OSStatInit();
#endif

	sem_GPS  = OSSemCreate(0);
	sem_can_rx = OSSemCreate(0);
	sem_sd = OSSemCreate(0);

	App_TaskCreate();

	USART_printf(USART2,"App_TaskCreate();\r\n");



	while(1)
	{


		Led_ON();
		OSTimeDlyHMSM(0,0,1,0);
		Led_OFF();
		OSTimeDlyHMSM(0,0,1,0);
	}
}

static void TaskGPS(void *p_arg)
{
	INT8U os_err=0;
	INT8U flag_time_adjust=0;
	NotSelect_GPS();
	Select_GPS();
	USART_printf(USART1,"$PSRF103,03,00,00,01*27\r\n");										//关闭GSV
	USART_printf(USART1,"$PSRF103,00,00,00,01*24\r\n");										//关闭GGA
	USART_printf(USART1,"$PSRF103,02,00,00,01*26\r\n");										//关闭GSA

	while (1)
	{
		OSSemPend(sem_GPS, 0, &os_err);			                                             //接收成功标志
		USART_printf(USART2,"\r\n%s\r\n",GPSTxBuffer);
		GPRMC_DAT();
		if(flag_time_adjust == 0 && GPS_Data.GPS_VA == 1)
		{
			Time_Adjust();
			flag_time_adjust=1;
		}


		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

static void TaskCanRx(void *p_arg)
{
	INT8U os_err=0;
	INT8U i = 0;
	INT8U j = 0;
	INT8U k = 0;
	INT8U flag_end;
	NUM_NODE=0;
	OSTimeDlyHMSM(0,0,0,500);
	for(i=1;i<=MAX_NODE;i++)
	{
		can_tx_msg.StdId = master_id+i;
		can_tx_msg.DLC = 8;
		can_tx_msg.RTR = CAN_RTR_DATA;		//0数据帧
		can_tx_msg.IDE  = CAN_ID_STD;	//0标准帧
		can_tx_msg.Data[0] = CAN_SIG;
		can_tx_msg.Data[1] = 0x01;
		can_tx_msg.Data[2] = 0;
		can_tx_msg.Data[3] = 0;
		can_tx_msg.Data[4] = 0;
		can_tx_msg.Data[5] = 0;
		can_tx_msg.Data[6] = 0;
		can_tx_msg.Data[7] = 0;
        CAN_Transmit(CAN1, &can_tx_msg);
		OSSemPend(sem_can_rx, 100, &os_err);
		if(os_err == OS_ERR_NONE)
		{
			ADD_NODE[++NUM_NODE] =   master_id+i;
		}

	}
	USART_printf(USART2,"NODE NUM: %d\r\n",NUM_NODE);
	while (1)
	{
		for(i=0;i<ADC_N;i++)
		{
			can_data[0][i*2]=AD_Value_Arg[i]>>8;
			can_data[0][i*2+1]=AD_Value_Arg[i];

		}

		for(i=1;i<=NUM_NODE;i++)
		{
			can_tx_msg.StdId = ADD_NODE[i];
			can_tx_msg.RTR = CAN_RTR_REMOTE;		//远程帧
			can_tx_msg.IDE  = CAN_ID_STD;			//标准帧
			do
			{

				CAN_Transmit(CAN1, &can_tx_msg);
				OSSemPend(sem_can_rx, 0, &os_err);
				if(can_rx_msg.StdId == master_id)
				{
					switch(can_rx_msg.Data[0] & 0xC0)
					{
						case CAN_STR:
									flag_end=0;
                                    k=0;
									break;
						case CAN_MID:
						case CAN_END:
									flag_end=1;
									break;

					}

					for(j=2;j<can_rx_msg.DLC;j++,k++)
					{
						can_data[i][k]=can_rx_msg.Data[j];

					}
				}
			}while(flag_end!=1);

		}
		OSSemPost(sem_sd);

		OSTimeDlyHMSM(0, 0, 0, 20);

	}
}


static void TaskData(void *p_arg)
{
	INT8U os_err=0;
	hrt_data_len =100;

	while (1)
	{

		hrt_data[0]=82;
		hrt_data[1]=81;
		hrt_data[2]=80;
		hrt_data[3]=hrt_data_len;



		OSTimeDlyHMSM(0, 0, 1, 0);

	}
}


static void TaskSD(void *p_arg)
{
	INT8U os_err=0;
	INT8U i;
	FATFS fs;																//驱动盘
										//文件路径
	FIL	file;																//文件指针
	FRESULT res;															//返回结果
	UINT br, bw;															//位置指针
													//挂载
	Time_Display(RTC_GetCounter());

	sd_len = 12;
	strcpy(path,"0:/");
	name[0]=systmtime.tm_hour/10+'0';
	name[1]=systmtime.tm_hour%10+'0';
	name[2]='_';
	name[3]=systmtime.tm_min/10+'0';
	name[4]=systmtime.tm_min%10+'0';
	name[5]='_';
	name[6]=systmtime.tm_sec/10+'0';
	name[7]=systmtime.tm_sec%10+'0';
	name[8]='\0';
	strcat(path,name);
	strcat(path,".dat");
        res = f_mount(0, &fs);
	while (1)
	{
		OSSemPend(sem_sd, 0, &os_err);

		res = f_open(&file, &path[0], FA_OPEN_ALWAYS  | FA_WRITE);
		if(res == FR_OK)
		{
			res = f_lseek(&file, file.fsize);								//指针指向文件尾
		}

		if(res == FR_OK)
		{
			f_write(&file, "FFF", 3 , &bw);
			f_write(&file, "G", 1 , &bw);
			f_write(&file, (void *)&GPS_Data.GPS_VA, 12 , &bw);
			for(i=0;i<=NUM_NODE;i++)
			{
				sd_buffer = (BYTE *)&can_data[i][0];
				f_write(&file, "FFF", 3 , &bw);
				f_write(&file, &i, 1 , &bw);
				f_write(&file, sd_buffer, sd_len , &bw);
			}
		}
		f_close(&file);
	}

}



static void App_TaskCreate()
{
	INT8U os_err=0;
	OSTaskCreateExt((void (*)(void *)) TaskSD,
					(void			*) 0,
					(OS_STK 		*) &TaskSDStk[TASK_SD_STK_SIZE - 1],
					(INT8U			 ) TASK_SD_PRIO ,
					(INT16U 		 ) TASK_SD_PRIO ,
					(OS_STK 		*) &TaskSDStk[0],
					(INT32U 		 ) TASK_SD_STK_SIZE,
					(void			*) 0,
					(INT16U 		 )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_SIZE > 1)
	OSTaskNameSet(TASK_SD_PRIO, "TaskSD", &os_err);
#endif


	OSTaskCreateExt((void (*)(void *)) TaskCanRx,
					(void			*) 0,
					(OS_STK 		*) &TaskCanRxStk[TASK_CAN_RX_STK_SIZE - 1],
					(INT8U			 ) TASK_CAN_RX_PRIO ,
					(INT16U 		 ) TASK_CAN_RX_PRIO ,
					(OS_STK 		*) &TaskCanRxStk[0],
					(INT32U 		 ) TASK_CAN_RX_STK_SIZE,
					(void			*) 0,
					(INT16U 		 )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_SIZE > 1)
	OSTaskNameSet(TASK_CAN_RX_PRIO, "TaskCanRx", &os_err);
#endif

	OSTaskCreateExt((void (*)(void *)) TaskGPS,
					(void			*) 0,
					(OS_STK 		*) &TaskGPSStk[TASK_GPS_STK_SIZE - 1],
					(INT8U			 ) TASK_GPS_PRIO ,
					(INT16U 		 ) TASK_GPS_PRIO ,
					(OS_STK 		*) &TaskGPSStk[0],
					(INT32U 		 ) TASK_GPS_STK_SIZE,
					(void			*) 0,
					(INT16U 		 )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_SIZE > 1)
	OSTaskNameSet(TASK_GPS_PRIO, "TaskGPS", &os_err);
#endif

/*	OSTaskCreateExt((void (*)(void *)) TaskData,
					(void			*) 0,
					(OS_STK 		*) &TaskDataStk[TASK_DATA_STK_SIZE - 1],
					(INT8U			 ) TASK_DATA_PRIO ,
					(INT16U 		 ) TASK_DATA_PRIO ,
					(OS_STK 		*) &TaskDataStk[0],
					(INT32U 		 ) TASK_DATA_STK_SIZE,
					(void			*) 0,
					(INT16U 		 )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_SIZE > 1)
	OSTaskNameSet(TASK_GPS_PRIO, "TaskGPS", &os_err);
#endif
*/
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

