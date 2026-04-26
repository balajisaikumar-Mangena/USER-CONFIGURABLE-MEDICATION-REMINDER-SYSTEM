/*====================================================================
 *                  RTC FUNCTION DECLARATIONS
 *====================================================================*/

extern int hour,min,sec;          // Global RTC time variables
extern int date,month,year;       // Global RTC date variables
extern int day;                   // Global RTC day variable

/*-------------------- INITIALIZATION --------------------*/
void RTC_Init(void);              // Initialize RTC module

/*-------------------- TIME FUNCTIONS --------------------*/
void GetRTCTimeInfo(int *,int *,int *);                // Read RTC time
void DisplayRTCTime(unsigned int,unsigned int,unsigned int); // Display time on LCD

/*-------------------- DATE FUNCTIONS --------------------*/
void GetRTCDateInfo(int *,int *,int *);                // Read RTC date
void DisplayRTCDate(unsigned int,unsigned int,unsigned int); // Display date on LCD

/*-------------------- SET FUNCTIONS --------------------*/
void SetRTCTimeInfo(unsigned int,unsigned int,unsigned int); // Set RTC time
void SetRTCDateInfo(unsigned int,unsigned int,unsigned int); // Set RTC date

/*-------------------- DAY FUNCTIONS --------------------*/
void GetRTCDay(int *);              // Read RTC day
void DisplayRTCDay(unsigned int);  // Display day
void SetRTCDay(unsigned int);      // Set day
void Day_OW(void);                 // Display day of week format

/*-------------------- DISPLAY --------------------*/
void DisplayRTC(void);             // Show full RTC (time + date + day)
