extern int hour,min,sec;
extern int date,month,year;
extern int day;

void RTC_Init(void);

void GetRTCTimeInfo(int *,int *,int *);
void DisplayRTCTime(unsigned int,unsigned int,unsigned int);

void GetRTCDateInfo(int *,int *,int *);
void DisplayRTCDate(unsigned int,unsigned int,unsigned int);

void SetRTCTimeInfo(unsigned int,unsigned int,unsigned int);
void SetRTCDateInfo(unsigned int,unsigned int,unsigned int);

void GetRTCDay(int *);
void DisplayRTCDay(unsigned int);
void SetRTCDay(unsigned int);
void Day_OW(void);

void DisplayRTC(void);
