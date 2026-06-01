# 💊 USER CONFIGURABLE MEDICATION REMINDER SYSTEM  
### ⏰ Embedded System using LPC2148 (ARM7)

---

## ✨ Overview  

The **User Configurable Medication Reminder System** is an embedded solution developed using the **LPC2148 ARM7 microcontroller** to ensure timely medication intake. It uses a **Real-Time Clock (RTC)** for accurate time tracking and continuously monitors scheduled medicine timings. The system generates alerts through a **buzzer and LCD display**, ensuring the user is notified at the correct time. A **keypad-based interface** enables easy configuration and management of medicine schedules. The design follows an **interrupt-driven approach**, providing reliable and real-time operation suitable for healthcare assistance..

---

## 🧩 Block Diagram  

<p align="center">
<img width="700" height="460" alt="Block Diagram" src="https://github.com/user-attachments/assets/6a8b0fe3-7684-400b-bf5d-158b2a809f9c" />

</p>

This diagram shows how all components are connected to the LPC2148 microcontroller.  
It illustrates the flow between keypad input, RTC processing, LCD output, and buzzer alert.

---

## 🔌 Circuit Diagram  

<p align="center">
<img width="1015" height="648" alt="Circuit Diagram" src="https://github.com/user-attachments/assets/8f5b9c8f-349a-4700-b9e0-49b78d28dc31" />

</p>

The circuit diagram represents the real hardware connections used in Proteus simulation.  
It includes LPC2148, LCD, keypad, switches, and buzzer wiring details.

---

# ✨ Features  

## ⏰ Real-Time Clock Integration  
- Utilizes the **LPC2148 RTC** for accurate timekeeping  
- Maintains **current time, date, and day** continuously  
- Ensures precise scheduling of medication alerts
<p align="center">
<img width="246" height="100" alt="image" src="https://github.com/user-attachments/assets/b2fcfde4-1a21-4839-a44a-8e571f518c87" />
</p>

---

## 💊 Dynamic Medicine Slot Management ⭐  
- Provides **3 default medicine slots**  
- Allows users to:
  - Edit existing slot timings  
  - Update schedules anytime  
- Supports **addition of up to 2 extra slots** based on user requirement  
- Handles **up to 5 total medicine slots** efficiently  
<p align="center">
<img width="246" height="100" alt="Add Med Slot UI" src="https://github.com/user-attachments/assets/3f22ee4f-a70d-403a-9b77-d3d72c478040" />

</p>

---

## 📟 Main Menu Medicine Schedule Display ⭐  
- Displays **current time along with all active medicine slots**  
- Enables users to view full schedule directly from the main screen  
- Automatically updates when slots are added or modified  

---

## 🔔 Smart Alert System  
- Continuously compares RTC time with configured medicine slots  
- Triggers alerts exactly when scheduled time matches  
- Ensures timely medication reminders without manual checking
 <p align="center">
 <img width="246" height="100" alt="TAke Medicine" src="https://github.com/user-attachments/assets/3246bf50-8e0a-4458-aab6-9afeaa083f5f" />

 </p>

---

## 🔊 Audio-Visual Notification  
- Buzzer alert for immediate attention  
- LCD display messages for clear instructions  
- Dual notification improves reliability  

---

## 🔢 User-Friendly Keypad Interface  
- Simple keypad-based navigation  
- Allows:
  - Adding new slots  
  - Editing existing timings  
  - Navigating menus easily
    <p align="center">
    <img width="221" height="221" alt="image" src="https://github.com/user-attachments/assets/39688ab3-a0b9-4f9e-9408-40538c01dd11" />
    </p>  

---

## ⚙️ Menu-Driven User Interface  
- Structured and intuitive menu system  
- Includes options for:
  - Time/Date setup  
  - Medicine slot management  
- Easy interaction for all users

---

## 🚨 Interrupt-Based Quick Access  
- Uses External Interrupt (EINT) for instant response  
- Enables quick entry into edit/configuration mode  
- Improves system responsiveness  

---

## ❌ Missed Dose Indication  
- Detects when a scheduled medicine is not taken  
- Displays warning message on LCD  
- Helps improve medication adherence
<p align="center">
<img width="242" height="96" alt="image" src="https://github.com/user-attachments/assets/64669f04-d783-4024-a85a-0b59ac33a806" />
</p>  

---

## 🔄 Real-Time Monitoring System  
- Continuously monitors all active medicine slots  
- Fully automated operation  
- No manual intervention required  

---


## ⚙️ Hardware Setup  

### 🔘 Switch 1 (Edit Mode)

Switch1 is connected to an external interrupt pin (**EINT0**) of the LPC2148 microcontroller.  
When the user presses this switch, the system immediately interrupts normal operation and enters **Edit Mode**.
<p align="center">
<img width="317" height="208" alt="SW1_Edit_mode" src="https://github.com/user-attachments/assets/c120d629-088b-463a-b004-57110c06acf3" />


</p>

In Edit Mode, the user can:

- Modify the **RTC time and date**
- Configure **medicine schedules**
- Add, edit, or update medicine slots
- Navigate through menu options using the keypad

This interrupt-based approach ensures that the user can access configuration settings **at any time**, without affecting the continuous RTC monitoring process.

After completing the configuration, the system safely returns to normal operation and resumes real-time monitoring.

---

### 🔘 Switch 2 (Stop Alert)

Switch2 is connected to an external interrupt pin (**EINT1**) of the LPC2148 microcontroller.  
When a medicine reminder is triggered, the buzzer starts alerting the user continuously.
<p align="center">
<img width="287" height="185" alt="SW2_stop_alert" src="https://github.com/user-attachments/assets/ea839bc3-cc6c-4f9c-b0a3-11c7ff2fb050" />

</p>

By pressing Switch2, the system immediately:

- Stops the **buzzer alert**
- Clears the **reminder message** from the LCD
- Confirms that the medicine has been taken

This interrupt-driven mechanism ensures **instant response**, allowing the user to acknowledge the alert without delay.

Additionally, after stopping the alert, the system automatically:

- Updates the current medicine status as **"Taken"**
- Displays the **next upcoming medicine schedule**
- Resumes normal RTC monitoring operation
<p align="center">
<img width="246" height="100" alt="Next Med display" src="https://github.com/user-attachments/assets/97e2d158-d44a-492a-9159-f98413512020" />


</p>

If the user does not press Switch2 within a predefined time, the system can automatically stop the alert and continue operation.

---

### 🎮 Controls Info

This screen provides a quick reference for keypad operations used in the system.  
Users can navigate menus, modify values, and confirm actions using the defined keys.

- **4 / 6** → Move between menu options (Left / Right)  
- **2 / 8** → Increment / Decrement values (Up / Down)  
- **=** → Confirm selection / Save settings  
- **C** → Exit current menu or cancel operation
<p align="center">
<img width="238" height="96" alt="image" src="https://github.com/user-attachments/assets/8727e338-af51-489d-940f-d06e9e798d9d" />
</p> 

This interface ensures smooth and intuitive interaction with the system.

---

## 🔄 System Workflow  

1. Initialize LCD, RTC, keypad, interrupts  
2. Display current time  
3. Enter Edit Mode using Switch1  
4. Configure medicine timings  
5. Monitor RTC continuously  
6. Trigger alert when time matches  
7. Stop alert using Switch2  
8. Display next medicine  

---

## 🧠 Working Principle  

- Switch1 → Enter setup mode  
- Keypad → Configure time & medicine  
- RTC → Provides real-time clock  
- Controller → Compares time  
- Match → Alert triggered 🔔  
- Switch2 → Stops alert  

---

## 🎮 Keypad Controls  

| Key | Function |
|-----|---------|
| 6 | Next Menu |
| 4 | Previous Menu |
| 8 | Decrement |
| 2 | Increment |
| = | OK / Save & Back |
| C | Exit |
| 5 | Controls |

---

## 📁 Project Structure  

```
USER-CONFIGURABLE-MEDICATION-REMINDER-SYSTEM
│
├── src
├── include
├── images
├── proteus
└── README.md
```

---

## 🎯 Advantages  

✔️ Helps patients take medicine on time  
✔️ Easy to configure  
✔️ Useful for elderly care  
✔️ Low cost embedded system  
✔️ Real-time monitoring  

---

## 👨‍💻 Author  

**Mangena Balaji Sai Kumar**

---

## ⭐ Support  

If you like this project, give it a ⭐ on GitHub!
