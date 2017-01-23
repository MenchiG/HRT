#ifdef 	_GLOBALS_
#define EXT
#else
#define EXT extern
#endif

#define Led_ON()   GPIO_ResetBits(GPIOB, GPIO_Pin_10)  	      //LED1亮
#define Led_OFF()  GPIO_SetBits(GPIOB, GPIO_Pin_10) 	      //LED1灭





#define uchar unsigned char


/************************************************
DATA
************************************************/
EXT u8 hrt_data[176];
EXT vu8 hrt_data_len;
EXT vu16 hrt_data_RPM;

/************************************************
2401
************************************************/
#define Select_NRF()     GPIO_ResetBits(GPIOA, GPIO_Pin_15)	  //NRF24L01 选择
#define NotSelect_NRF()  GPIO_SetBits(GPIOA, GPIO_Pin_15)	  //NRF24L01 禁止

#define TX_ADR_WIDTH    5                                     	//TX(RX) 地址宽度
#define TX_PLOAD_WIDTH  32                      			  //发送帧长度
//EXT unsigned char it_msg,led_flash,rx_count,Rx_Succ;

EXT unsigned char TX_ADDRESS0[TX_ADR_WIDTH]; // Define a static TX address
EXT unsigned char TX_ADDRESS1[TX_ADR_WIDTH]; // Define a static TX address
EXT unsigned char TX_ADDRESS2[TX_ADR_WIDTH]; // Define a static TX address
EXT unsigned char TX_ADDRESS3[TX_ADR_WIDTH]; // Define a static TX address
EXT unsigned char TX_ADDRESS4[TX_ADR_WIDTH]; // Define a static TX address
EXT unsigned char TX_ADDRESS5[TX_ADR_WIDTH]; // Define a static TX address

EXT unsigned char rx_buf[32];				 //接收缓冲区
EXT unsigned char tx_buf[32];				 //发送缓冲区
EXT unsigned char status_buf[20];			 //状态字缓冲区
EXT unsigned char flag,nrf_baud,nrf_Pipe,nrf_Pipe_r;

EXT unsigned char nrf_ok;

/************************************************
GPS
************************************************/
#define GPS_DATALENGTH 100
#define Select_GPS()     GPIO_SetBits(GPIOA, GPIO_Pin_8)	  //GPS选择
#define NotSelect_GPS()  GPIO_ResetBits(GPIOA, GPIO_Pin_8)	  //GPS 禁止

EXT OS_EVENT * sem_GPS;
EXT vu8  GPSRxBuffer[GPS_DATALENGTH];							//接收缓存
EXT vu8  GPSTxBuffer[GPS_DATALENGTH];
EXT	vu8  Rec_Len;
EXT GPS_InitTypeDef GPS_Data;

/************************************************
CAN
************************************************/
#define MAX_NODE 10

#define master_id 0x070
#define slave_id 0x070

#define CAN_END 0xC0
#define CAN_STR 0x40
#define CAN_MID 0X80
#define CAN_SIG 0X00

EXT OS_EVENT * sem_can_rx;

EXT CanRxMsg can_rx_msg;
EXT CanTxMsg can_tx_msg;
EXT INT8U can_address;
EXT vu8 can_data[MAX_NODE][20];
EXT u8 NUM_NODE;
EXT u8 ADD_NODE[MAX_NODE];



/************************************************
SD
************************************************/

EXT OS_EVENT * sem_sd;

EXT BYTE *sd_buffer;					//缓冲区
EXT UCHAR sd_head;
EXT UCHAR sd_len;

EXT CHAR path[50],name[10];


/************************************************
ADC
************************************************/
#define ADC_N 5
#define ADC_M 30

EXT OS_EVENT * sem_adc;
EXT vu16 AD_Value[ADC_M][ADC_N];
EXT vu16 AD_Value_Arg[ADC_N];

/************************************************
RTC
************************************************/

EXT vu32 Online_Time;
EXT RTC_InitTypeDef systmtime;

/************************************************
BKP
************************************************/


#define BKP_Check BKP_DR1
#define BKP_On_time BKP_DR2
#define BKP_Run_time BKP_DR3
#define BKP_FLASH_Page BKP_DR4
#define BKP_FLASH_BLOCK BKP_DR5
/************************************************
MMA
************************************************/
EXT INT8U mma_mode;
