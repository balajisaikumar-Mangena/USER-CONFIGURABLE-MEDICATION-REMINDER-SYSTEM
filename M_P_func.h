extern volatile unsigned int M_H1;
extern volatile unsigned int M_min1;
extern volatile unsigned int M_H2;
extern volatile unsigned int M_min2;
extern volatile unsigned int M_H3;
extern volatile unsigned int M_min3;
extern volatile unsigned int M_H4;
extern volatile unsigned int M_min4;
extern volatile unsigned int M_H5;
extern volatile unsigned int M_min5;

extern unsigned int flag;

void display_LCD(unsigned int num);
void Display_Menu(void);
void Display_MED1(void);
void Display_MED2(void);
void Display_MED3(void);
void Display_MED4(void);
void Display_MED5(void);

void DisplayMED1(void);
void DisplayMED2(void);
void DisplayMED3(void);

void Arrow_Init(void);
void Arrow_Init1(void);
void Arrow_UI(void);
void LCD_Control_UI(void);

void Increment_Value(unsigned int *value, unsigned int max);
void Decrement_Value(unsigned int *value, unsigned int max);

void Hour(void);
void Min(void);
void Sec(void);
void Date(void);
void Month(void);
void Year(void);
void Day(void);
void Med1_Hour(void);
void Med1_Min(void);
void Med2_Hour(void);
void Med2_Min(void);
void Med3_Hour(void);
void Med3_Min(void);
void Med4_Hour(void);
void Med4_Min(void);
void Med5_Hour(void);
void Med5_Min(void);

void Edit_RTC_Time(void);
void Edit_RTC_Date(void);
void Edit_RTC_Day(void);
void Edit_Med1_Time(void);
void Edit_Med2_Time(void);
void Edit_Med3_Time(void);
void Edit_Med4_Time(void);
void Edit_Med5_Time(void);

void Show_Next_Med(void);
void Alert(void);

void MENU(void);
void MRS(void);
void Edit_menu(void);
