#ifndef __RTC_H__
#define	__RTC_H__


typedef struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
}RTC_InitTypeDef;

void GregorianDay(struct rtc_time * tm);
unsigned int mktimev(struct rtc_time *tm);
void to_tm(unsigned int tim, struct rtc_time * tm);
void Time_Adjust(void);
void Time_Display(unsigned int TimeVar);

#endif
