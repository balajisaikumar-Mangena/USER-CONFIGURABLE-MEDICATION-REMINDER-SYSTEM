# USER CONFIGURABLE MEDICATION REMINDER SYSTEM

Embedded System Project using LPC2148 Microcontroller

---

# Introduction

The User Configurable Medication Reminder System is an embedded system designed to help users follow their medication schedule accurately. The system reminds users to take medicines at specific times using a buzzer alert and LCD display notification.

This project is implemented using the LPC2148 ARM7 microcontroller. The system integrates a Real Time Clock (RTC), 16x2 LCD display, keypad interface, and buzzer alert system.

Users can configure medicine schedules through a menu-driven interface. When the configured medicine time matches the RTC time, the system activates an alert and displays the next upcoming medicine time.

---

# Project Objectives

The objectives of this project are:

* To develop an embedded system that reminds users to take medicines on time.
* To allow users to configure medicine schedules using a keypad interface.
* To provide a simple LCD-based menu system for easy interaction.
* To support dynamic medicine slot configuration.
* To automatically determine and display the next upcoming medicine.

---

# System Features

## Real Time Clock Integration

* Maintains accurate system time using RTC.
* Tracks hours, minutes, seconds, date, and day.
* Continuously compares RTC time with configured medicine schedules.

## Medicine Reminder Alert

* Buzzer activates when medicine time matches the configured schedule.
* LCD displays reminder message indicating which medicine should be taken.
* Alert continues until the user acknowledges the reminder.

## LCD Based User Interface

* Uses a 16x2 LCD display for system interaction.
* Displays current time and date.
* Shows medicine schedules and menu options.
* Displays reminder notifications and next medicine time.

## Keypad Based Navigation

The keypad is used to navigate the menu system and configure settings.

Navigation keys used in the project:

* **6** : Move to next menu
* **4** : Move to previous menu
* **=** : Confirm or select option
* **C** : Exit or return to previous menu
* **5** : Display control key instructions

## User Configurable Medicine Slots

The system initially supports three medicine schedules.

Default medicine slots:

* MEDICINE 1
* MEDICINE 2
* MEDICINE 3

Additional optional slots:

* MEDICINE 4
* MEDICINE 5

Users can dynamically add additional medicine slots through the edit menu.

## Smart Next Medicine Detection

After a medicine is acknowledged, the system automatically determines the next upcoming medicine based on the nearest future schedule.

Example:

Current Time

```
14:30
```

Medicine Schedule

```
MED1 08:00
MED2 12:00
MED3 16:00
MED4 18:00
MED5 22:00
```

Displayed Output

```
NEXT MED 16:00
```

The system skips the current medicine and displays the nearest upcoming medicine.

## Automatic Daily Cycle

After the last medicine of the day, the system automatically selects the first medicine schedule for the next day.

---

# Hardware Components

The following hardware components are used in this project:

* LPC2148 ARM7 Microcontroller
* 16x2 LCD Display
* Matrix Keypad
* Real Time Clock (RTC)
* Buzzer
* 5V Regulated Power Supply

---

# Software Tools

The following development tools were used:

* **Keil µVision** – Embedded C development
* **Proteus** – Hardware simulation
* **Flash Magic** – Microcontroller programming

---

# Project Structure

The project is divided into multiple modules to maintain clean and structured code.

```
USER-CONFIGURABLE-MEDICATION-REMINDER-SYSTEM

Mini_Project.c
M_P_func.c
M_P_func.h

RTC_functions.c
RTC.h

EINT.c
EINT.h

LCD_functions.c
lcd.h

KPM_Func.c
KPM.h

delay_functions.c
delay.h
```

---

# Source File Description

## Mini_Project.c

This is the main program file that controls the overall execution of the system.

Responsibilities:

* Initializes LCD module
* Initializes keypad module
* Initializes RTC module
* Initializes interrupt system
* Displays project startup message
* Runs the main program loop
* Calls display and alert functions

Main functions called:

* Display_Menu()
* Alert()
* Edit_menu()

---

## M_P_func.c

This file contains the core logic of the medication reminder system.

Responsibilities include:

* Menu system implementation
* LCD user interface control
* Medicine schedule management
* Medicine reminder alert logic
* Medicine time editing functions
* Next medicine detection logic
* Dynamic medicine slot management

---

## M_P_func.h

Header file for the main project functions.

Contains declarations for major system functions including:

* Display_Menu()
* Alert()
* Edit_menu()
* Medicine display functions
* Time editing functions
* Menu control functions

---

## RTC_functions.c

Handles Real Time Clock operations.

Responsibilities:

* Initialize RTC module
* Read current time
* Read current date
* Display RTC information on LCD
* Update RTC registers

---

## RTC.h

Header file for RTC module.

Contains RTC variable definitions and function declarations.

---

## EINT.c

Handles external interrupt operations.

Responsibilities:

* Detect interrupt signals
* Detect acknowledgement switch press
* Stop buzzer alert when medicine is taken

---

## EINT.h

Header file for interrupt module containing interrupt declarations.

---

## LCD_functions.c

Contains functions used to control the LCD display.

Functions implemented:

* init_LCD()
* cmd_LCD()
* char_LCD()
* str_LCD()

These functions are used to display menus, system messages, and reminder alerts.

---

## lcd.h

Header file for LCD module containing LCD function declarations.

---

## KPM_Func.c

Handles keypad interface and key detection.

Functions implemented:

* init_KPM()
* Keypad_Scan()

This module allows the user to navigate menus and configure system settings.

---

## KPM.h

Header file for keypad module containing keypad function declarations.

---

## delay_functions.c

Implements delay functions used for system timing.

Functions implemented:

* delay_s()
* delay_ms()
* delay_us()

These delays are used for LCD timing and program execution control.

---

## delay.h

Header file containing delay function declarations.

---

# System Workflow

## System Initialization

When the system starts:

* LCD is initialized
* RTC module is started
* Keypad interface is configured
* Interrupt module is enabled
* System enters the main program loop

## Normal Operation

During normal operation:

* LCD displays the current time and date
* System rotates through medicine schedules
* User can enter edit mode using keypad

## Configuration Mode

Users can configure system parameters including:

* RTC Time
* RTC Date
* RTC Day
* Medicine schedules
* Additional medicine slots

## Medicine Reminder Operation

When RTC time matches a medicine schedule:

* LCD displays reminder message
* Buzzer alert is activated
* User presses acknowledgement switch
* Alert stops after confirmation

## Next Medicine Display

After confirmation:

* System calculates nearest upcoming medicine
* LCD displays next medicine schedule

---

# Advantages

* Helps users maintain proper medication schedules
* Simple and easy to use interface
* Configurable medicine schedules
* Dynamic medicine slot addition
* Automatic next medicine detection
* Low power embedded system design
* Useful for elderly patient care
---

# Example Output

When medicine time arrives:

```
TAKE MEDICINE 2
PRESS SW2 STOP
```

After acknowledgement:

```
MEDICINE 2 TAKEN
NEXT MED 16:30
```

---

# Author

Mangena Balaji Sai Kumar
