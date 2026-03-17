#include <lpc21xx.h>
#include "delay.h"
#include "lcd.h"
#include "KPM.h"
#include "M_P_func.h"

#define DATA (0xFF)
#define RS (1<<8)
#define RW (1<<9)
#define EN (1<<10)

#define RTC_ENABLE (1<<0)
#define RTC_RESET  (1<<1)

#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

extern int hour,min,sec;
extern int date,month,year;
extern int day;

char week[][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

void RTC_Init(void){
	CCR = RTC_RESET;
	PREINT  = 457;
	PREFRAC = 25024;
	CCR = RTC_ENABLE;
}

void GetRTCTimeInfo(int *h,int *m,int *s){
	*h = HOUR;
	*m = MIN;
	*s = SEC;
}

void DisplayRTCTime(unsigned int h,unsigned int m,unsigned int s){
	h = HOUR;
	m = MIN;
	s = SEC;
	
	char_LCD((h/10)+48);
	char_LCD((h%10)+48);
	char_LCD(':');

	char_LCD((m/10)+48);
	char_LCD((m%10)+48);
	char_LCD(':');

	char_LCD((s/10)+48);
	char_LCD((s%10)+48);
}

void GetRTCDateInfo(int *d,int *m,int *y){
	*d = DOM;
	*m = MONTH;
	*y = YEAR;
}

void Day_OW(){
				unsigned int temp = DOW;
				cmd_LCD(0xC0);
				str_LCD("   DOW : ");
				if(temp == 00) str_LCD("SUN");
				if(temp == 01) str_LCD("MON");
				if(temp == 02) str_LCD("TUE");
				if(temp == 03) str_LCD("WED");
				if(temp == 04) str_LCD("THU");
				if(temp == 05) str_LCD("FRI");
				if(temp == 06) str_LCD("SAT");
				str_LCD("    ");
}

void DisplayRTCDate(unsigned int d,unsigned int m,unsigned int y){
	d = DOM;
	m = MONTH;
	y = YEAR;
	char_LCD((d/10)+48);
	char_LCD((d%10)+48);
	char_LCD('/');

	char_LCD((m/10)+48);
	char_LCD((m%10)+48);
	char_LCD('/');

	char_LCD((y/1000)%10 + 48);
	char_LCD((y/100)%10 + 48);
	char_LCD((y/10)%10 + 48);
	char_LCD((y%10) + 48);
}

void SetRTCTimeInfo(unsigned int h,unsigned int m,unsigned int s){
	HOUR = h;
	MIN  = m;
	SEC  = s;
}

void SetRTCDateInfo(unsigned int d,unsigned int m,unsigned int y){
	DOM = d;
	MONTH = m;
	YEAR = y;
}

void GetRTCDay(int *d){
	*d = DOW;
}

void DisplayRTCDay(unsigned int d){
	d = DOW;
	str_LCD(week[d]);
}

void SetRTCDay(unsigned int d){
	DOW = d;
}

void DisplayRTC(void){
	Arrow_Init();
	GetRTCTimeInfo(&hour,&min,&sec);
	cmd_LCD(0x80);
	str_LCD("TIME  : ");
	DisplayRTCTime(hour,min,sec);

	GetRTCDateInfo(&date,&month,&year);
	cmd_LCD(0xC0);
	DisplayRTCDate(date,month,year);

	GetRTCDay(&day);
	cmd_LCD(0xcA);
	str_LCD(" ");
	DisplayRTCDay(day);
	str_LCD(" ");
	char_LCD(0);
}
