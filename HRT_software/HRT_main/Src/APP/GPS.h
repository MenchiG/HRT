#ifndef __GPS_H__
#define	__GPS_H__

typedef struct{
volatile unsigned char Year;
volatile unsigned char Mouth;
volatile unsigned char Day;
volatile unsigned char Hour;
volatile unsigned char Min;
volatile unsigned char Sec;
volatile unsigned char GPS_VA;
volatile unsigned char latitude_dir;
volatile unsigned char latitude_h;
volatile unsigned char latitude_m;
volatile unsigned char latitude_s;
volatile unsigned char latitude_ss;
volatile unsigned char longitude_dir;
volatile unsigned char longitude_h;
volatile unsigned char longitude_m;
volatile unsigned char longitude_s;
volatile unsigned char longitude_ss;
volatile unsigned char Speed;
volatile unsigned short int Speed_dir;
}GPS_InitTypeDef;

void GPRMC_DAT();
#endif /* __USART1_H */