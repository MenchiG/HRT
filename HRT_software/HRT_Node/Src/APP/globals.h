#ifdef GLOBALS
#define EXT
#else
#define EXT extern
#endif

#define LED_ON() GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define LED_OFF() GPIO_SetBits(GPIOA, GPIO_Pin_12)

#define CAN_ON() GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define CAN_OFF() GPIO_SetBits(GPIOA, GPIO_Pin_3)

/************************************************
CAN
************************************************/
#define master_id 0x070
#define node_id 0x071

#define CAN_END 0xC0
#define CAN_STR 0x40
#define CAN_MID 0X80
#define CAN_SIG 0X00


EXT OS_EVENT * sem_can_rx;
EXT CanRxMsg can_rx_msg;
EXT CanTxMsg can_tx_msg;
EXT INT8U can_address;

/************************************************
ADC
************************************************/
#define ADC_N 4
#define ADC_M 30

EXT OS_EVENT * sem_adc;
EXT vu16 AD_Value[ADC_M][ADC_N];
EXT vu16 AD_Value_Arg[ADC_N];
/************************************************
TIM
************************************************/

EXT vu16 Freq_Value[2];

