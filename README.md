# Robust Multi-Factor Door Lock System

This project implements a secure access control system using **Arduino**, a **4x4 keypad**, **Bluetooth module**, **servo motor**, and **LCD screen**.

## Key Features
- **PIN Authentication**: Unlock the door by entering the correct PIN.
- **Dynamic PIN Generation**: If the wrong PIN is entered, a new random PIN is sent via Bluetooth, which must be entered to unlock.
- **Real-time Feedback**: LCD displays success or failure, and a buzzer sounds for incorrect PIN entries.
- **Enhanced Security**: Combines traditional PIN entry with dynamic PIN recovery for stronger protection.

## Components Used
- Arduino Uno
- 4x4 Keypad
- Bluetooth Module (HC-05)
- Servo Motor
- LCD Display
- Buzzer

## How to Use
1. Enter the correct PIN on the keypad to unlock the door.
2. If the wrong PIN is entered, a new PIN will be sent to the connected Bluetooth device.
3. The user must enter the new PIN via the keypad to unlock the door.

## Setup
1. Connect all components as per the wiring diagram.
2. Upload the code to the Arduino board.
3. Use the Bluetooth module to receive the dynamic PIN when needed.
## Block Diagram
![diagram-export-11-05-2025-01_22_54](https://github.com/user-attachments/assets/eb077d92-1b4d-4be6-bae1-928d479562a8)



