# USER CONFIGURABLE MEDICATION REMINDER SYSTEM (LPC2148 | Embedded C)

The User Configurable Medication Reminder System is an embedded system developed using the LPC2148 ARM7 microcontroller. The system uses a Real Time Clock (RTC), 16x2 LCD display, keypad interface, and buzzer alert to remind users to take medicines at scheduled times.

The system allows users to configure medicine timings and automatically determines the next upcoming medicine based on the nearest future schedule.

---

# Project Overview

The medication reminder system helps users maintain proper medicine schedules.

When the configured medicine time matches the RTC time, the system performs the following operations:

* Activates a buzzer alert
* Displays a medicine reminder message on the LCD
* Waits for the user acknowledgement
* Stops the alert after user confirmation
* Displays the next nearest medicine time

The system allows users to configure up to five medicine schedules.

---

# Features

## Real Time Clock Integration

* Maintains accurate system time using RTC
* Compares real time clock with configured medicine schedules
* Ensures precise triggering of reminders

## Medicine Alert System

* Buzzer activates when a medicine time is reached
* LCD displays the medicine reminder message
* User acknowledgement stops the buzzer alert

## LCD Based User Interface

* 16x2 LCD used for displaying system information
* Shows real time clock data
* Displays menu options and reminder messages

## Keypad Navigation

The keypad is used to navigate through the menu and configure system settings.

Navigation keys:

* 6 : Move to next menu
* 4 : Move to previous menu
* = : Select or confirm option
* C : Exit or return to previous menu
* 5 : Enter configuration menu

## Dynamic Medicine Slot Configuration

The system initially supports three medicine schedules.

Default medicine slots:

* MED1
* MED2
* MED3

Additional optional slots:

* MED4
* MED5

Users can enable additional slots through the configuration menu.

## Smart Next Medicine Detection

The system automatically determines the next medicine based on the nearest upcoming time.

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

Displayed Result

```
NEXT MED 16:00
```

The system skips the current medicine and displays the nearest future medicine.

## Automatic Daily Operation

* After the last medicine of the day, the system automatically selects the first medicine for the next day
* Ensures continuous operation without manual reset

---

# Hardware Components

The system uses the following hardware components:

* LPC2148 ARM7 Microcontroller
* 16x2 LCD Display
* Matrix Keypad
* Real Time Clock Module
* Buzzer
* 5V Regulated Power Supply

---

# Software Tools

The following development tools were used for this project:

* Keil uVision for embedded C programming
* Proteus for hardware simulation
* Flash Magic for microcontroller programming

---

# Project Structure

```
USER-CONFIGURABLE-MEDICATION-REMINDER-SYSTEM
│
├── main.c
├── lcd.c
├── lcd.h
├── keypad.c
├── keypad.h
├── rtc.c
├── rtc.h
├── delay.c
├── delay.h
├── interrupt.c
├── interrupt.h
│
├── Alert.c
├── Menu.c
├── Edit_Menu.c
│
├── Proteus Simulation
│   └── MedicationReminder.pdsprj
│
└── README.md
```

---

# System Workflow

## System Initialization

When the system starts:

* LCD is initialized
* RTC module is started
* Keypad interface is enabled
* Buzzer output is configured

## Normal Operation

* LCD displays current RTC time
* System rotates through configured medicine schedules

## Configuration Mode

Users can configure system parameters including:

* RTC Time
* RTC Date
* RTC Day
* Medicine timing schedules
* Additional medicine slots

## Medicine Reminder Process

When the RTC time matches a configured medicine time:

* LCD displays the medicine reminder message
* Buzzer alert is activated
* User presses acknowledgement switch to stop the alert

## Next Medicine Display

After the user confirms the medicine has been taken:

* The system calculates the nearest upcoming medicine
* LCD displays the next medicine time

---

# User Configuration

Users can configure the following parameters using the keypad interface:

* RTC Time
* RTC Date
* RTC Day
* Medicine 1 Time
* Medicine 2 Time
* Medicine 3 Time
* Optional Medicine 4 Slot
* Optional Medicine 5 Slot

---

# Advantages

* Helps users follow correct medication schedules
* Simple and easy to operate user interface
* Supports configurable medicine schedules
* Automatically detects the nearest upcoming medicine
* Low power embedded system
* Useful for elderly patient monitoring

---

# Future Improvements

Possible improvements for this system include:

* GSM based medicine alerts
* Mobile application connectivity
* Bluetooth configuration
* Voice reminder alerts
* Missed medicine detection
* Battery backup support

---

# Example Output

When medicine time arrives:

```
TAKE MEDICINE 2
PRESS SW2 STOP
```

After confirmation:

```
MEDICINE 2 TAKEN
NEXT MED 16:30
```

---

# Author

Mangena Balaji Sai Kumar
