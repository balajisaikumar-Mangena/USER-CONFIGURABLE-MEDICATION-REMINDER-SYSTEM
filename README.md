# USER CONFIGURABLE MEDICATION REMINDER SYSTEM

Embedded System Project using **LPC2148 ARM7 Microcontroller**

---

# Introduction

The **User Configurable Medication Reminder System** is an embedded system designed to help users take medicines at the correct time.

The system continuously monitors time using the **Real Time Clock (RTC)**. When the scheduled medicine time arrives, the system alerts the user using a **buzzer** and displays a reminder message on the **LCD screen**.

Users can configure medicine schedules using a **keypad-based menu system**.

This project is implemented using the **LPC2148 ARM7 microcontroller**, simulated in **Proteus**, and programmed using **Embedded C in Keil µVision**.

---

# Project Objectives

The objectives of this project are:

* Develop a medicine reminder system using an embedded microcontroller.
* Display real-time clock information on an LCD.
* Allow users to configure medicine timings.
* Alert the user when medicine time occurs.
* Provide a simple menu-based user interface.

---

# Block Diagram
![Block Diagram](https://github.com/user-attachments/assets/672a50f9-da82-43e6-8e83-17c803dab419)

The system consists of the following modules:

- LPC2148 Microcontroller – Controls the entire system
- RTC – Maintains current time and date
- LCD Display – Shows time, menus, and reminders
- Keypad – Used for menu navigation and configuration
- Buzzer – Generates alert when medicine time arrives
- Switch1 – Used to enter edit mode
- Switch2 – Used to stop the reminder alert

---

# Circuit Diagram
![Circuit Diagram](https://github.com/user-attachments/assets/170aceb9-d40d-484a-9267-bf7e11c892e2)


The circuit was designed and simulated in Proteus.  
It connects the LPC2148 microcontroller with the LCD, keypad, buzzer, and switches.
---

# Hardware Components

The following hardware components are used:

* LPC2148 ARM7 Microcontroller
* 16x2 LCD Display
* 4x4 Matrix Keypad
* Buzzer
* Push Button Switches
* RTC module
* Power Supply

---

# Software Tools

The following software tools were used to develop this project:

* **Keil µVision** – Embedded C development
* **Proteus** – Circuit simulation
* **Flash Magic** – Microcontroller programming

---

# System Features

## Real Time Clock Monitoring
<img width="246" height="102" alt="image" src="https://github.com/user-attachments/assets/43ce321e-0510-42bf-acb2-93fcc9c92ba6" />


The system continuously reads time from the RTC and displays it on the LCD.

Example display:

TIME : 06:31:17
16/03/2026 MON

---

## User Configurable Medicine Schedule

Users can configure medicine timings using the keypad through the edit menu.

Default medicine slots:

* MEDICINE 1
* MEDICINE 2
* MEDICINE 3

Additional slots can also be added.

---

## Medicine Reminder Alert

When the RTC time matches a medicine schedule:

* LCD displays reminder message
* Buzzer alert starts

Example:

TAKE MEDICINE 1
PRESS SW2 STOP

The alert stops when the user presses **Switch2**.

---

## Next Medicine Detection

After a medicine is taken, the system automatically shows the next upcoming medicine time.

Example:

MEDICINE 1 TAKEN
NEXT MED 12:30

---

## LCD Menu Interface

The system provides a simple **LCD based menu system**.

Example menu:

* EDIT MODE *
  RTC   MED   EXIT

---

# Keypad Controls
<div style="margin-left:2000px;">
<img width="219" height="214" alt="image" src="https://github.com/user-attachments/assets/0d8d8c03-999d-4111-be3f-505df8a83725" />
</div>

| Key | Function                           |
| --- | -----------------------------------|
| 6   | Move to next menu                  |
| 4   | Move to previous menu              |
| =   | OK / Save & back to previous menu  |
| C   | Exit                               |
| 5   | Show control instructions          |

---

# System Screenshots

### Edit Mode Menu
![Edit Mode display](https://github.com/user-attachments/assets/b5edb5ee-6d27-4699-992d-a79fbbb910ca)


### Keypad Control Instructions
<img width="244" height="101" alt="Screenshot 2026-03-16 212938" src="https://github.com/user-attachments/assets/229dee61-4b65-4c1f-8fde-94f6fb1dddc7" />



### Add Medicine Slot
![Add Med Slot UI](https://github.com/user-attachments/assets/f3f3db1d-2493-4d02-921e-22fcdf6953a4)


### Slot Added Successfully
![Slot ADD Succefully UI](https://github.com/user-attachments/assets/e720f64c-2340-45b8-b374-8afde9b521f3)


### Medicine Reminder Alert
![TAke Medicine](https://github.com/user-attachments/assets/0b383fac-5880-4d42-8462-3b5267f187b3)


### Next Medicine Display
![Next Med display](https://github.com/user-attachments/assets/bc935136-97f7-4fc3-a495-5c678a167f07)

---

# Project Structure
```
USER-CONFIGURABLE-MEDICATION-REMINDER-SYSTEM
│
├── src
│   ├── Mini_Project.c
│   ├── M_P_func.c
│   ├── RTC_functions.c
│   ├── LCD_functions.c
│   ├── KPM_Func.c
│   ├── EINT.c
│   └── delay_functions.c
│
├── include
│   ├── M_P_func.h
│   ├── RTC.h
│   ├── lcd.h
│   ├── KPM.h
│   ├── EINT.h
│   └── delay.h
│
├── images
│   ├── Block Diagram.jpg
│   ├── Circuit Diagram.jpg
│   ├── Edit Mode display.jpg
│   ├── Controls UI.jpg
│   ├── Add Med Slot UI.jpg
│   ├── Slot ADD Succefully UI.jpg
│   ├── TAke Medicine.jpg
│   └── Next Med display.jpg
│
├── proteus
│   └── Medication_Reminder_System.pdsprj
│
└── README.md
```
# System Workflow

1. System initializes LCD, RTC, keypad, and interrupts.
2. LCD displays the current time and date.
3. User can enter **Edit Mode** using Switch1.
4. User configures medicine schedules using keypad.
5. System continuously compares RTC time with medicine schedules.
6. When medicine time occurs, buzzer alert activates.
7. User presses **Switch2** to stop alert.
8. System calculates and displays the **next upcoming medicine**.

---

# Advantages

* Helps users take medicines on time
* Easy to configure schedules
* Simple menu-based interface
* Automatic next medicine detection
* Useful for elderly patient care

---

# Author

**Mangena Balaji Sai Kumar**
