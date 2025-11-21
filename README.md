# STM32H562RGV6 Display Board - GUI Implementation

## Screenshots

### Temperature Control Screen
<img src="photos/sc3.jpg" alt="Temperature Control Screen" width="400">

### Schedule Screen
<img src="photos/sc2.jpg" alt="Schedule Screen" width="400">

### Add Schedule Screen
<img src="photos/sc1.jpg" alt="Add Schedule Screen" width="400">

## Overview
This project implements a touchscreen GUI for a temperature control system on an STM32H5 microcontroller with an ILI9488 480x320 TFT display.

## Screens Implemented

### 1. Temperature Control Screen (`Plot_Touchscreen_Temperature_Screen`)
The main screen for temperature control and monitoring.

**Features:**
- Large, centered temperature display with precise positioning
- Increase/Decrease buttons (+/-) on the right side
- Knob indicator with visual logo showing rotary encoder support
- Three bottom navigation buttons: Schedule, On/Off, Reset
- Header with date and time display
- Clean, minimal design with simple gray color scheme

**Layout:**
- Header: 50px height with title, date (left), and time (right)
- Temperature box: 330×110px, black background with white text
- Adjustment buttons: 52px height each, equal size, vertically centered
- Bottom buttons: 140px width each, evenly spaced

**Color Scheme:**
- Background: NAVY_BLUE
- Header: DARK_BLUE
- Buttons: GRAY/DARK_GRAY with WHITE text
- Temperature display: BLACK background, WHITE text

### 2. Schedule Screen (`Plot_Touchscreen_Schedule_Screen`)
Displays scheduled temperature changes in a 2-column grid layout.

**Features:**
- 2×2 grid of schedule entry boxes
- Each box shows: Time → Temperature
- Two bottom buttons: Back (to Temperature screen) and Add New (to Add Schedule screen)
- Consistent styling matching the Temperature screen

**Layout:**
- Schedule boxes: ~219px × 80px each
- 15px spacing between boxes
- 12px internal padding for text
- Text scale: 2.5 for better readability

**Color Scheme:**
- Background: NAVY_BLUE
- Schedule boxes: GRAY background
- Text: WHITE for time/temperature, LIGHT_GRAY for arrows

### 3. Add Schedule Screen (`Plot_Touchscreen_AddSchedule_Screen`)
Screen for creating new schedule entries.

**Features:**
- Three input cards: Time, Temperature, and Notes (optional)
- Vertically centered layout
- Two bottom buttons: Cancel and Save (both return to Schedule screen)
- Clean, organized design

### 4. Schedule Wizard (`Plot_Touchscreen_ScheduleWizard_*`)
Multi-step flow for creating schedule entries with weekday, time, temperature, and name. The wizard consists of 4 separate screens for a clean, focused experience.

**Step 1 – Day Selection (`Plot_Touchscreen_ScheduleWizard_Day`)**
- Single row of 7 day buttons (Mon–Sun) for compact layout
- Selected day highlighted with DARK_GRAY background
- Shows selected day name below buttons
- Knob hint with icon: "Use knob to change day"
- Navigation: `Back`, `Next`

**Step 2 – Time Selection (`Plot_Touchscreen_ScheduleWizard_Time`)**
- Large time display panel (HH:MM format) with black background
- Increase/Decrease buttons (+/-) on the right side for time adjustment
- Consistent styling with temperature screen
- Knob hint with icon: "Use knob to adjust time"
- Navigation: `Back`, `Next`

**Step 3 – Temperature Selection (`Plot_Touchscreen_ScheduleWizard_Temperature`)**
- Large temperature display panel with centered value
- Increase/Decrease buttons (+/-) on the right side
- Temperature shown in format: XX.X°C
- Knob hint with icon: "Use knob to adjust temperature"
- Navigation: `Back`, `Next`

**Step 4 – Name & Confirm (`Plot_Touchscreen_ScheduleWizard_Name`)**
- Name input card for schedule entry name
- Two summary cards showing selected day/time and temperature
- Compact layout with smaller text for better fit
- Navigation: `Back`, `Save`

**Design Features:**
- All steps use consistent button colors (GRAY background, WHITE text)
- Step labels use smaller text (scale 2.5) for cleaner appearance
- Knob hints include visual icon (rotary encoder symbol) for consistency
- Spacing optimized for better visual balance
- All screens share the same navy/gray color palette

## Navigation Flow

```
Temperature Screen (Main)
    ↓ [Schedule button]
Schedule Screen
    ↓ [Add New button]        ↑ [Back button]
Schedule Wizard – Step 1 (Day Selection)
    ↓ [Next]
Wizard – Step 2 (Time Selection)
    ↓ [Next]
Wizard – Step 3 (Temperature Selection)
    ↓ [Next]
Wizard – Step 4 (Name & Confirm)
    ↓ [Save]
Schedule Screen
    ↑ [Back button]
Temperature Screen
```

**Navigation Buttons:**
- **Temperature Screen**: Schedule → Schedule Screen, On/Off → Toggle system, Reset → Reset temperature
- **Schedule Screen**: Back → Temperature Screen, Add New → Schedule Wizard Step 1
- **Schedule Wizard**:
  - Step 1 (Day): Back → Schedule Screen, Next → Step 2
  - Step 2 (Time): Back → Step 1, Next → Step 3
  - Step 3 (Temperature): Back → Step 2, Next → Step 4
  - Step 4 (Name & Confirm): Back → Step 3, Save → Schedule Screen
- **Add Schedule Screen**: Cancel → Schedule Screen, Save → Schedule Screen

## Files Modified

- `Core/Src/GUI.c`: Main GUI implementation
- `Core/Inc/GUI.h`: Function declarations
- `Core/Src/main.c`: Screen initialization and testing

## Usage

The screens are called in sequence in `main.c`:
```c
// Main screens
Plot_Touchscreen_Temperature_Screen(16, 2025, 11, 25, 8, 32, 25);
Plot_Touchscreen_Schedule_Screen(2025, 11, 26, 19, 47, 33);
Plot_Touchscreen_AddSchedule_Screen(2025, 11, 26, 19, 47, 33);

// Schedule Wizard (4-step flow)
Plot_Touchscreen_ScheduleWizard_Day(2025, 11, 26, 19, 47, 33, 0);
Plot_Touchscreen_ScheduleWizard_Time(2025, 11, 26, 19, 47, 33, 0, 9, 0);
Plot_Touchscreen_ScheduleWizard_Temperature(2025, 11, 26, 19, 47, 33, 250, 0, 9, 0);
Plot_Touchscreen_ScheduleWizard_Name(2025, 11, 26, 19, 47, 33, "Morning Warmup", 250, 0, 9, 0);
```
