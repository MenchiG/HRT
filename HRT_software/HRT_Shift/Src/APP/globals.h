#ifdef GLOBALS
#define EXT
#else
#define EXT extern
#endif

#define LED_LED1_ON()   GPIO_ResetBits(GPIOB, GPIO_Pin_3)  	       //LED1  ÁÁ
#define LED_LED1_OFF()  GPIO_SetBits(GPIOB, GPIO_Pin_3) 	   		//LED1  Ãð

#define LED_LED2_ON()   GPIO_ResetBits(GPIOB, GPIO_Pin_4)  	       //LED2 ÁÁ
#define LED_LED2_OFF()  GPIO_SetBits(GPIOB, GPIO_Pin_4 ) 	          //LED2  Ãð

#define LED_LED3_ON()   GPIO_ResetBits(GPIOB, GPIO_Pin_5 )  	       //LED3  ÁÁ
#define LED_LED3_OFF()  GPIO_SetBits(GPIOB, GPIO_Pin_5 ) 	   	      //LED3  Ãð

#define SIGNAL_UP_ON()   GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define SIGNAL_UP_OFF()   GPIO_SetBits(GPIOA, GPIO_Pin_3)

#define SIGNAL_DOWN_ON()   GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SIGNAL_DOWN_OFF()   GPIO_SetBits(GPIOA, GPIO_Pin_4)

#define SIGNAL_Clu_ON()   GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define SIGNAL_Clu_OFF() GPIO_SetBits(GPIOA, GPIO_Pin_5)


#define CAN_DISABLE() GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define CAN_ENABLE() GPIO_SetBits(GPIOB, GPIO_Pin_6)

#define FILTER_UP (OS_FLAGS)3											//0Î»Îª²Ù×÷Î» 1up 2down 3return
#define FILTER_DOWN (OS_FLAGS)5
#define FILTER_RETURN (OS_FLAGS)9

#define MODE_NORMAL   0x01
#define MODE_ACCEL   0x03

#define ID_ECU 0x0E8

EXT OS_EVENT *Sem_neu;
EXT OS_FLAG_GRP *Sem_F;

EXT OS_EVENT *Mbox_Com1;

EXT OS_EVENT * sem_can_rx;
EXT CanRxMsg can_rx_msg;
EXT CanTxMsg can_tx_msg;

EXT u8 uMode;

EXT s16 s16_time[8];

#define UPT1 1
#define UPT2 2
#define UPT3 3

#define DOT1 4
#define DOT2 5
#define DOT3 6

#define NEU 7

#define DLY 0

EXT u16 control_counter_UP,control_counter_DOWN;
EXT u8 RxCounter1;
EXT u8 CanRxCounter;

EXT u16 RPM;

