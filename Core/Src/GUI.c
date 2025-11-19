/*
 * GUI.C
 *
 *  Created on: May 31, 2025
 *      Author: Martin
 */

#include <stdio.h>

#include "ili9488.h"
#include "global_variables.h"

#include "main.h" // for HAL_Delay

/*
void Plot_GUI_screen_1()
{

        uint16_t trace_color = GREEN;  // Green
        uint16_t via_color   = WHITE;  // White
        uint16_t comp_color  = BLUE;  // Blue

        // Trace 1: horizontal
        ILI9488_drawThickLine(30, 100, 200, 100, 3, trace_color);
        HAL_Delay(100);

        // Via 1
        ILI9488_drawCircleOutline(30, 100, 6, 3, via_color);
        HAL_Delay(100);

        // Via 2
        ILI9488_drawCircleOutline(200, 100, 6, 3, via_color);
        HAL_Delay(100);

        // Trace 2: vertical from first trace
        ILI9488_drawThickLine(200, 100, 200, 200, 3, trace_color);
        HAL_Delay(100);

        // Trace 3: curve approximation (segment steps)
        ILI9488_drawThickLine(200, 200, 180, 220, 3, trace_color);
        HAL_Delay(100);
        ILI9488_drawThickLine(180, 220, 150, 220, 3, trace_color);
        HAL_Delay(100);

        // Via 3
        ILI9488_drawCircleOutline(150, 220, 6, 3, via_color);
        HAL_Delay(100);

        // Component 1: IC rectangle
        ILI9488_FilledRectangle(80, 140, 140, 170, comp_color);
        HAL_Delay(100);

        // Pins (horizontal lines)
        for (int i = 0; i < 4; i++) {
            ILI9488_drawThickLine(80 - 10, 145 + i * 6, 80, 145 + i * 6, 2, trace_color);
            HAL_Delay(50);
            ILI9488_drawThickLine(140, 145 + i * 6, 140 + 10, 145 + i * 6, 2, trace_color);
            HAL_Delay(50);
        }

        // Component 2: capacitor
        ILI9488_Rect(220, 180, 20, 40, comp_color);
        HAL_Delay(100);
        ILI9488_drawThickLine(210, 200, 220, 200, 2, trace_color);
        HAL_Delay(100);
        ILI9488_drawThickLine(240, 200, 260, 200, 2, trace_color);
        HAL_Delay(100);

        // Final via (bottom trace)
        ILI9488_drawCircleOutline(260, 200, 6, 3, via_color);
        HAL_Delay(100);


}
*/

void Plot_GUI_screen_1()
{

    uint16_t trace_color = LIGHTER_GREEN; //
    uint16_t via_color = LIGHTER_GREEN;   //
    uint16_t comp_color = LIGHTER_GREEN;  //
    uint16_t label_color = WHITE;         //
    uint16_t battery_color = WHITE;       //

    // Clear screen with black (optional)
    ILI9488_FillScreen(DARKER_GREEN);
    HAL_Delay(200);

    // --- Top bus traces ---
    ILI9488_drawThickLine(20, 40, 460, 40, 3, trace_color);
    HAL_Delay(50);
    ILI9488_drawThickLine(20, 60, 460, 60, 3, trace_color);
    HAL_Delay(50);

    // Vias on top bus
    for (int x = 40; x <= 440; x += 80)
    {
        ILI9488_drawCircleOutline(x, 40, 5, 2, via_color);
        HAL_Delay(25);
        ILI9488_drawCircleOutline(x, 60, 5, 2, via_color);
        HAL_Delay(25);
    }

    // Trace 1: horizontal
    ILI9488_drawThickLine(30, 100, 100, 100, 3, trace_color);
    HAL_Delay(100);

    // Via 1
    ILI9488_drawCircleOutline(30, 100, 6, 3, via_color);
    HAL_Delay(100);

    // Via 2
    ILI9488_drawCircleOutline(100, 100, 6, 3, via_color);
    HAL_Delay(100);

    // Trace 2: vertical from first trace
    ILI9488_drawThickLine(30, 100, 30, 200, 3, trace_color);
    HAL_Delay(100);

    // Trace 3: curve approximation (segment steps)
    ILI9488_drawThickLine(30, 200, 80, 220, 3, trace_color);
    HAL_Delay(100);
    ILI9488_drawThickLine(80, 220, 130, 220, 3, trace_color);
    HAL_Delay(100);

    // Via 3
    ILI9488_drawCircleOutline(130, 220, 6, 3, via_color);
    HAL_Delay(100);

    ILI9488_DrawBatterySymbol(370, 80, 100, battery_color); // Draw battery symbol
    HAL_Delay(100);

    /*
    // Component 1: IC rectangle
    ILI9488_FilledRectangle(80, 140, 140, 170, comp_color);
    HAL_Delay(100);
*/
    /*
    // Pins (horizontal lines)
    for (int i = 0; i < 4; i++) {
        ILI9488_drawThickLine(80 - 10, 145 + i * 6, 80, 145 + i * 6, 2, trace_color);
        HAL_Delay(50);
        ILI9488_drawThickLine(140, 145 + i * 6, 140 + 10, 145 + i * 6, 2, trace_color);
        HAL_Delay(50);
    }
*/
    /*
    // --- IC block left ---
    ILI9488_FilledRectangle(50, 100, 130, 150, comp_color);
    HAL_Delay(50);
    ILI9488_DrawString(60, 110, 1, "U1", label_color, comp_color);
    HAL_Delay(50);

    // Pins and traces from U1
    for (int i = 0; i < 4; i++) {
        int y = 110 + i * 8;
        ILI9488_drawThickLine(130, y, 170, y, 2, trace_color);
        HAL_Delay(25);
        ILI9488_drawCircleOutline(170, y, 4, 2, via_color);
        HAL_Delay(25);
    }
*/
    // --- Capacitor ---
    /*
        ILI9488_Rect(200, 90, 20, 40, comp_color);
        ILI9488_DrawString(202, 100, 1, "C1", label_color, 0x0000);
        HAL_Delay(50);
        ILI9488_drawThickLine(190, 110, 200, 110, 2, trace_color);
        HAL_Delay(50);
        ILI9488_drawThickLine(220, 110, 240, 110, 2, trace_color);
        HAL_Delay(50);
    */
    // --- Resistor ---
    /*
    ILI9488_Rect(280, 130, 30, 10, comp_color);
    ILI9488_DrawString(282, 132, 1, "R1", label_color, comp_color);
    HAL_Delay(50);
    ILI9488_drawThickLine(260, 135, 280, 135, 2, trace_color);
    HAL_Delay(50);
    ILI9488_drawThickLine(310, 135, 330, 135, 2, trace_color);
    HAL_Delay(50);
*/
    // --- IC block right ---
    /*
    ILI9488_FilledRectangle(350, 180, 430, 230, comp_color);
    ILI9488_DrawString(370, 190, 1, "U2", label_color, comp_color);
    HAL_Delay(50);

    // U2 pin traces
    for (int i = 0; i < 4; i++) {
        int y = 190 + i * 8;
        ILI9488_drawThickLine(350, y, 310, y, 2, trace_color);
        HAL_Delay(25);
        ILI9488_drawCircleOutline(310, y, 4, 2, via_color);
        HAL_Delay(25);
    }

    // --- Diagonal traces across screen ---
    ILI9488_drawThickLine(20, 300, 460, 240, 3, trace_color);
    HAL_Delay(50);
    ILI9488_drawThickLine(460, 300, 20, 240, 3, trace_color);
    HAL_Delay(50);
*/
    /*
    // --- Vias on bottom edge ---
    for (int x = 60; x <= 420; x += 90) {
        ILI9488_drawCircleOutline(x, 310, 5, 2, via_color);
        HAL_Delay(25);
    }
*/
    // --- Text logo ---
    ILI9488_DrawString(140, 180, 12, "BMS", label_color, DARKER_GREEN);
    HAL_Delay(25);
    ILI9488_DrawString(140, 130, 3, "designed by", label_color, DARKER_GREEN);
    HAL_Delay(25);
    ILI9488_DrawString(140, 80, 3, "Martin Novak", label_color, DARKER_GREEN);
    HAL_Delay(25);

    // HAL_Delay(100);
}

void Plot_GUI_screen_2()
{
    char buffer[50]; // buffer for strings

    // background color
    // ILI9488_FillScreen(BLACK);
    // draw strings
    // display X,Y 0,0 is bottom right corner
    // X is horizontal
    // Y is vertical
    sprintf(buffer, "#2");
    ILI9488_DrawString(400, 280, 4, buffer, WHITE, BLACK);

    sprintf(buffer, "SOC[%%] %02d.%0d", BMS_no1.SOC / 10, (BMS_no1.SOC % 10));
    ILI9488_DrawString(10, 200, 4, buffer, WHITE, BLACK);

    sprintf(buffer, "V[V] %02d.%0d   ", BMS_no1.Stack_Voltage_mV / 1000, (BMS_no1.Stack_Voltage_mV % 1000));
    ILI9488_DrawString(10, 150, 4, buffer, WHITE, BLACK);

    sprintf(buffer, "I[A] %02d.%0d   ", BMS_no1.current_userA / 100, (BMS_no1.current_userA % 100));
    ILI9488_DrawString(10, 100, 4, buffer, WHITE, BLACK);

    sprintf(buffer, "P[W] %05d", BMS_no1.Power_W);
    ILI9488_DrawString(10, 50, 4, buffer, WHITE, BLACK);

    sprintf(buffer, "T[C] %02d.%0d", BMS_no1.Internal_Temperature_C / 10, (BMS_no1.Internal_Temperature_C % 10));
    ILI9488_DrawString(10, 10, 4, buffer, WHITE, BLACK);
}

// screen with cell voltages
void Plot_GUI_screen_3()
{
    char buffer[50]; // buffer for strings

    // background color
    // ILI9488_FillScreen(BLUE);
    // draw strings
    sprintf(buffer, "#3");
    ILI9488_DrawString(400, 280, 4, buffer, WHITE, BLUE);

    sprintf(buffer, "cells [mV]");
    ILI9488_DrawString(10, 280, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "01: %04d", BMS_no1.Cell_1_voltage_mV);
    ILI9488_DrawString(10, 240, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "02: %04d", BMS_no1.Cell_2_voltage_mV);
    ILI9488_DrawString(10, 210, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "03: %04d", BMS_no1.Cell_3_voltage_mV);
    ILI9488_DrawString(10, 180, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "04: %04d", BMS_no1.Cell_4_voltage_mV);
    ILI9488_DrawString(10, 150, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "05: %04d", BMS_no1.Cell_5_voltage_mV);
    ILI9488_DrawString(10, 120, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "06: %04d", BMS_no1.Cell_6_voltage_mV);
    ILI9488_DrawString(10, 90, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "07: %04d", BMS_no1.Cell_7_voltage_mV);
    ILI9488_DrawString(10, 60, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "08: %04d", BMS_no1.Cell_8_voltage_mV);
    ILI9488_DrawString(10, 30, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "09: %04d", BMS_no1.Cell_9_voltage_mV);
    ILI9488_DrawString(250, 240, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "10: %04d", BMS_no1.Cell_10_voltage_mV);
    ILI9488_DrawString(250, 210, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "11: %04d", BMS_no1.Cell_11_voltage_mV);
    ILI9488_DrawString(250, 180, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "12: %04d", BMS_no1.Cell_12_voltage_mV);
    ILI9488_DrawString(250, 150, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "13: %04d", BMS_no1.Cell_13_voltage_mV);
    ILI9488_DrawString(250, 120, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "14: %04d", BMS_no1.Cell_14_voltage_mV);
    ILI9488_DrawString(250, 90, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "15: %04d", BMS_no1.Cell_15_voltage_mV);
    ILI9488_DrawString(250, 60, 3, buffer, WHITE, BLUE);

    sprintf(buffer, "16: %04d", BMS_no1.Cell_16_voltage_mV);
    ILI9488_DrawString(250, 30, 3, buffer, WHITE, BLUE);
}

// gui screen for testing, displays some rectangles, circles, pixels, character and string
void Plot_GUI_screen_4()
{
    char buffer[50]; // buffer for strings

    // background color
    ILI9488_FillScreen(DARKER_GREEN); // darker green
    // testing
    // ILI9488_DrawPixel(50, 100, WHITE);
    // ILI9488_DrawPixel(350, 100, WHITE);
    // ILI9488_DrawPixel(10, 100, WHITE);
    // ILI9488_DrawPixel(50, 100, WHITE);

    // ILI9488_V_line(0, 0, 200, RED);
    // ILI9488_H_line(0, 0, 200, GREEN);

    // ILI9488_Rect(100,100,50,100,YELLOW);

    // ILI9488_Rect_3(50,30,50,100,YELLOW);
    ILI9488_FilledRectangle(100, 200, 150, 300, RED);

    // ILI9488_drawThickLine(0, 0, 150, 200, 4, BLUE);

    ILI9488_drawCircleOutline(200, 200, 20, 3, WHITE);

    // ILI9488_DrawPixel(50, 30, WHITE);
    ILI9488_FilledRectangle(250, 250, 300, 350, RED);

    ILI9488_FilledRectangle(250, 50, 300, 150, WHITE);
    // ILI9488_Rect(250, 250, 50, 100, YELLOW);

    ILI9488_DrawBatterySymbol(50, 30, 100, WHITE); // Draw battery symbol

    ILI9488_DrawChar(240, 10, 5, 'R', WHITE, RED);

    ILI9488_DrawString(10, 280, 3, "test", WHITE, BLUE);
}

// ============================================================================
// TOUCHSCREEN UI SCREENS
// ============================================================================

/**
 * @brief Helper function to draw a modern button with border
 * @param x1, y1: Top-left corner
 * @param x2, y2: Bottom-right corner
 * @param text: Button text
 * @param bg_color: Background color
 * @param text_color: Text color
 * @param border_color: Border color
 * @param scale: Text scale
 */
static void Draw_Modern_Button(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                               const char *text, uint16_t bg_color, uint16_t text_color,
                               uint16_t border_color, uint8_t scale)
{
    // Draw button background
    ILI9488_FilledRectangle(x1, y1, x2, y2, bg_color);

    // Calculate text position (centered)
    uint16_t text_x = x1 + ((x2 - x1) / 2) - (strlen(text) * 6 * scale / 2);
    uint16_t text_y = y1 + ((y2 - y1) / 2) - (8 * scale / 2);

    // Draw button text
    ILI9488_DrawString(text_x, text_y, scale, (char *)text, text_color, bg_color);
}

/**
 * @brief Draws the temperature control screen
 * @param temperature: Current temperature value (in 0.1°C units, e.g., 215 = 21.5°C)
 * @param year, month, day: Date values
 * @param hour, minute, second: Time values
 *
 * Screen layout:
 * - Header with date/time display
 * - Large temperature display at top center
 * - Increase button (top right)
 * - Decrease button (bottom right)
 * - Schedule button (bottom left)
 * - Back/Reset button (bottom center)
 * - Knob indicator hint
 */
void Plot_Touchscreen_Temperature_Screen(int16_t temperature,
                                         uint16_t year, uint8_t month, uint8_t day,
                                         uint8_t hour, uint8_t minute, uint8_t second)
{
    char buffer[50];

    // Screen dimensions
    const uint16_t SCREEN_WIDTH = 480;
    const uint16_t SCREEN_HEIGHT = 320;
    const uint16_t MARGIN = 15; // Increased for better visual spacing
    const uint16_t HEADER_HEIGHT = 50;
    const uint16_t BUTTON_SPACING = 15; // Increased for better button separation
    const uint16_t BOTTOM_BUTTON_HEIGHT = 45;
    const uint16_t BOTTOM_BUTTON_Y = SCREEN_HEIGHT - BOTTOM_BUTTON_HEIGHT - MARGIN;

    // Clear screen with dark blue background
    ILI9488_FillScreen(NAVY_BLUE);

    // ===== HEADER =====
    // Title bar with precise height
    ILI9488_FilledRectangle(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, DARK_BLUE);
    ILI9488_DrawString(MARGIN, 8, 2, "Temperature Control", WHITE, DARK_BLUE);

    // Date display (left side of header)
    sprintf(buffer, "%04d-%02d-%02d", year, month, day);
    ILI9488_DrawString(MARGIN, 30, 2, buffer, LIGHT_GRAY, DARK_BLUE);

    // Time display (right side of header)
    sprintf(buffer, "%02d:%02d:%02d", hour, minute, second);
    uint16_t time_x = SCREEN_WIDTH - MARGIN - (strlen(buffer) * 6 * 2);
    ILI9488_DrawString(time_x, 30, 2, buffer, WHITE, DARK_BLUE);

    // ===== MAIN TEMPERATURE DISPLAY =====
    // Temperature box: left-aligned with margin, leaving space for +/- buttons
    const uint16_t TEMP_BOX_X1 = MARGIN;
    const uint16_t TEMP_BOX_Y1 = HEADER_HEIGHT + MARGIN;
    const uint16_t TEMP_BOX_WIDTH = 330;  // Adjusted to leave proper space for +/- buttons
    const uint16_t TEMP_BOX_HEIGHT = 110; // Adjusted for better proportions
    const uint16_t TEMP_BOX_X2 = TEMP_BOX_X1 + TEMP_BOX_WIDTH;
    const uint16_t TEMP_BOX_Y2 = TEMP_BOX_Y1 + TEMP_BOX_HEIGHT;

    // Background for temperature display
    ILI9488_FilledRectangle(TEMP_BOX_X1, TEMP_BOX_Y1, TEMP_BOX_X2, TEMP_BOX_Y2, BLACK);

    // Temperature value (precisely centered in box)
    int16_t temp_whole = temperature / 10;
    int16_t temp_decimal = temperature % 10;
    if (temp_decimal < 0)
        temp_decimal = -temp_decimal;

    // Calculate precise center position for temperature text
    sprintf(buffer, "%d", temp_whole);
    uint16_t whole_width = strlen(buffer) * 6 * 8;
    sprintf(buffer, ".%d", temp_decimal);
    uint16_t decimal_width = strlen(buffer) * 6 * 8;
    uint16_t unit_width = 6 * 6 + 25; // "C" + "o" symbol width

    uint16_t total_width = whole_width + decimal_width + unit_width + 10;
    uint16_t temp_text_x = TEMP_BOX_X1 + (TEMP_BOX_WIDTH / 2) - (total_width / 2);
    uint16_t temp_text_y = TEMP_BOX_Y1 + (TEMP_BOX_HEIGHT / 2) - (8 * 8 / 2) - 5;

    sprintf(buffer, "%d", temp_whole);
    ILI9488_DrawString(temp_text_x, temp_text_y, 8, buffer, WHITE, BLACK);

    // Decimal point and digit
    sprintf(buffer, ".%d", temp_decimal);
    uint16_t decimal_x = temp_text_x + whole_width;
    ILI9488_DrawString(decimal_x, temp_text_y, 8, buffer, WHITE, BLACK);

    // Degree symbol and unit
    uint16_t unit_x = decimal_x + decimal_width + 10;
    ILI9488_DrawString(unit_x, temp_text_y + 5, 6, "C", WHITE, BLACK);
    ILI9488_DrawString(unit_x + 25, temp_text_y + 12, 3, "o", WHITE, BLACK);

    // Label below temperature (perfectly centered, with more space from temperature)
    // Calculate bottom of temperature text (degree symbol extends to temp_text_y + 12 + 8*3 = temp_text_y + 36)
    uint16_t temp_bottom_y = temp_text_y + (8 * 8) + 5; // Bottom of main temperature text + degree symbol space
    uint16_t label_spacing = 20;                        // More space between temperature and label
    sprintf(buffer, "Set Temperature");
    uint16_t label_x = TEMP_BOX_X1 + (TEMP_BOX_WIDTH / 2) - (strlen(buffer) * 6 * 2 / 2);
    uint16_t label_y = temp_bottom_y + label_spacing;
    ILI9488_DrawString(label_x, label_y, 2, buffer, LIGHT_GRAY, BLACK);

    // ===== INCREASE/DECREASE BUTTONS =====
    // Positioned to the right of temperature box with proper spacing, contained within temperature box height
    const uint16_t ADJ_BTN_X1 = TEMP_BOX_X2 + BUTTON_SPACING;
    const uint16_t ADJ_BTN_X2 = SCREEN_WIDTH - MARGIN;
    const uint16_t ADJ_BTN_WIDTH = ADJ_BTN_X2 - ADJ_BTN_X1;
    const uint16_t ADJ_BTN_HEIGHT = 52; // Smaller buttons - equal size for both, fits within temp box
    const uint16_t ADJ_BTN_SPACING = 8; // Spacing between + and - buttons
    const uint16_t ADJ_BTN_TOTAL_HEIGHT = (ADJ_BTN_HEIGHT * 2) + ADJ_BTN_SPACING;
    // Ensure buttons fit within temperature box height and don't affect other elements
    const uint16_t ADJ_BTN_START_Y = TEMP_BOX_Y1 + (TEMP_BOX_HEIGHT / 2) - (ADJ_BTN_TOTAL_HEIGHT / 2);

    // Increase button (top) - smaller, equal size, centered with temperature box
    uint16_t inc_btn_y1 = ADJ_BTN_START_Y;
    uint16_t inc_btn_y2 = inc_btn_y1 + ADJ_BTN_HEIGHT;
    Draw_Modern_Button(ADJ_BTN_X1, inc_btn_y1, ADJ_BTN_X2, inc_btn_y2,
                       "+", DARK_GRAY, WHITE, LIGHT_GRAY, 5);

    // Decrease button (bottom) - smaller, equal size, centered with temperature box
    uint16_t dec_btn_y1 = inc_btn_y2 + ADJ_BTN_SPACING;
    uint16_t dec_btn_y2 = dec_btn_y1 + ADJ_BTN_HEIGHT;
    Draw_Modern_Button(ADJ_BTN_X1, dec_btn_y1, ADJ_BTN_X2, dec_btn_y2,
                       "-", DARK_GRAY, WHITE, LIGHT_GRAY, 5);

    // ===== KNOB INDICATOR =====
    // Positioned below temperature box with proper spacing and visible logo
    uint16_t knob_y = TEMP_BOX_Y2 + 40; // Increased spacing from temperature box
    uint16_t knob_icon_x = MARGIN;
    uint16_t knob_icon_y = knob_y + 8; // Center icon with text
    uint16_t knob_icon_radius = 12;

    // Draw knob icon (circle representing rotary encoder)
    ILI9488_drawCircleOutline(knob_icon_x + knob_icon_radius, knob_icon_y, knob_icon_radius, 2, WHITE);
    ILI9488_drawCircleOutline(knob_icon_x + knob_icon_radius, knob_icon_y, knob_icon_radius - 4, 1, LIGHT_GRAY);
    // Draw center dot
    ILI9488_FilledRectangle(knob_icon_x + knob_icon_radius - 2, knob_icon_y - 2,
                            knob_icon_x + knob_icon_radius + 2, knob_icon_y + 2, WHITE);

    // Draw text next to icon (more visible)
    uint16_t knob_text_x = knob_icon_x + (knob_icon_radius * 2) + 10;
    ILI9488_DrawString(knob_text_x, knob_y, 2, "Use knob to adjust temperature", WHITE, NAVY_BLUE);

    // ===== BOTTOM BUTTON ROW =====
    // Three buttons: Schedule (navigate to Schedule screen), On/Off (toggle system), Reset (functionality)
    const uint16_t BOTTOM_BUTTON_WIDTH = 140;
    const uint16_t TOTAL_BUTTON_WIDTH = (BOTTOM_BUTTON_WIDTH * 3) + (BUTTON_SPACING * 2);
    const uint16_t BOTTOM_START_X = (SCREEN_WIDTH - TOTAL_BUTTON_WIDTH) / 2; // Centered

    // Schedule button (left) - navigates to Schedule Screen
    uint16_t sched_btn_x1 = BOTTOM_START_X;
    uint16_t sched_btn_x2 = sched_btn_x1 + BOTTOM_BUTTON_WIDTH;
    Draw_Modern_Button(sched_btn_x1, BOTTOM_BUTTON_Y, sched_btn_x2, BOTTOM_BUTTON_Y + BOTTOM_BUTTON_HEIGHT,
                       "Schedule", GRAY, WHITE, LIGHT_GRAY, 3);

    // On/Off button (center) - toggles temperature control system
    uint16_t onoff_btn_x1 = sched_btn_x2 + BUTTON_SPACING;
    uint16_t onoff_btn_x2 = onoff_btn_x1 + BOTTOM_BUTTON_WIDTH;
    Draw_Modern_Button(onoff_btn_x1, BOTTOM_BUTTON_Y, onoff_btn_x2, BOTTOM_BUTTON_Y + BOTTOM_BUTTON_HEIGHT,
                       "On/Off", GRAY, WHITE, LIGHT_GRAY, 3);

    // Reset button (right) - functionality button
    uint16_t reset_btn_x1 = onoff_btn_x2 + BUTTON_SPACING;
    uint16_t reset_btn_x2 = reset_btn_x1 + BOTTOM_BUTTON_WIDTH;
    Draw_Modern_Button(reset_btn_x1, BOTTOM_BUTTON_Y, reset_btn_x2, BOTTOM_BUTTON_Y + BOTTOM_BUTTON_HEIGHT,
                       "Reset", GRAY, WHITE, LIGHT_GRAY, 3);
}

/**
 * @brief Draws the schedule/schemes screen
 * @param year, month, day: Date values
 * @param hour, minute, second: Time values
 *
 * Screen layout:
 * - Header with title and date/time
 * - List of scheduled temperature changes
 * - Back button to return to temperature screen
 */
void Plot_Touchscreen_Schedule_Screen(uint16_t year, uint8_t month, uint8_t day,
                                      uint8_t hour, uint8_t minute, uint8_t second)
{
    char buffer[50];

    // Screen dimensions (matching temperature screen)
    const uint16_t SCREEN_WIDTH = 480;
    const uint16_t SCREEN_HEIGHT = 320;
    const uint16_t MARGIN = 15;
    const uint16_t HEADER_HEIGHT = 50;
    const uint16_t BUTTON_SPACING = 15;
    const uint16_t BOTTOM_BUTTON_HEIGHT = 45;
    const uint16_t BOTTOM_BUTTON_Y = SCREEN_HEIGHT - BOTTOM_BUTTON_HEIGHT - MARGIN;

    // Clear screen with dark blue background
    ILI9488_FillScreen(NAVY_BLUE);

    // ===== HEADER =====
    // Title bar with precise height (matching temperature screen)
    ILI9488_FilledRectangle(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, DARK_BLUE);
    ILI9488_DrawString(MARGIN, 8, 2, "Temperature Schedule", WHITE, DARK_BLUE);

    // Date display (left side of header)
    sprintf(buffer, "%04d-%02d-%02d", year, month, day);
    ILI9488_DrawString(MARGIN, 30, 2, buffer, LIGHT_GRAY, DARK_BLUE);

    // Time display (right side of header)
    sprintf(buffer, "%02d:%02d:%02d", hour, minute, second);
    uint16_t time_x = SCREEN_WIDTH - MARGIN - (strlen(buffer) * 6 * 2);
    ILI9488_DrawString(time_x, 30, 2, buffer, WHITE, DARK_BLUE);

    // ===== SCHEDULE LIST =====
    // Schedule entries displayed as boxes in 2 columns with improved styling
    uint16_t list_y_start = HEADER_HEIGHT + MARGIN;
    uint16_t box_spacing = 15; // Increased spacing between boxes
    uint16_t box_height = 80;  // Taller boxes for better appearance
    uint16_t box_padding = 12; // Internal padding for text
    uint16_t available_width = SCREEN_WIDTH - (MARGIN * 2);
    uint16_t box_width = (available_width - box_spacing) / 2; // Two boxes per row

    // Row 1: Entry 1 (left) and Entry 2 (right)
    uint16_t row1_y = list_y_start;

    // Schedule entry 1 (left box)
    uint16_t box1_x1 = MARGIN;
    uint16_t box1_x2 = box1_x1 + box_width;
    uint16_t box1_y1 = row1_y;
    uint16_t box1_y2 = box1_y1 + box_height;
    ILI9488_FilledRectangle(box1_x1, box1_y1, box1_x2, box1_y2, GRAY);
    ILI9488_DrawString(box1_x1 + box_padding, box1_y1 + 20, 2.5, "09:00", WHITE, GRAY);
    ILI9488_DrawString(box1_x1 + box_padding, box1_y1 + 40, 2.5, "->", LIGHT_GRAY, GRAY);
    sprintf(buffer, "%d.%d C", 22, 0);
    ILI9488_DrawString(box1_x1 + box_padding, box1_y1 + 60, 2.5, buffer, WHITE, GRAY);

    // Schedule entry 2 (right box)
    uint16_t box2_x1 = box1_x2 + box_spacing;
    uint16_t box2_x2 = box2_x1 + box_width;
    uint16_t box2_y1 = row1_y;
    uint16_t box2_y2 = box2_y1 + box_height;
    ILI9488_FilledRectangle(box2_x1, box2_y1, box2_x2, box2_y2, GRAY);
    ILI9488_DrawString(box2_x1 + box_padding, box2_y1 + 20, 2.5, "12:00", WHITE, GRAY);
    ILI9488_DrawString(box2_x1 + box_padding, box2_y1 + 40, 2.5, "->", LIGHT_GRAY, GRAY);
    sprintf(buffer, "%d.%d C", 24, 5);
    ILI9488_DrawString(box2_x1 + box_padding, box2_y1 + 60, 2.5, buffer, WHITE, GRAY);

    // Row 2: Entry 3 (left) and Entry 4 (right)
    uint16_t row2_y = row1_y + box_height + box_spacing;

    // Schedule entry 3 (left box)
    uint16_t box3_x1 = MARGIN;
    uint16_t box3_x2 = box3_x1 + box_width;
    uint16_t box3_y1 = row2_y;
    uint16_t box3_y2 = box3_y1 + box_height;
    ILI9488_FilledRectangle(box3_x1, box3_y1, box3_x2, box3_y2, GRAY);
    ILI9488_DrawString(box3_x1 + box_padding, box3_y1 + 20, 2.5, "18:00", WHITE, GRAY);
    ILI9488_DrawString(box3_x1 + box_padding, box3_y1 + 40, 2.5, "->", LIGHT_GRAY, GRAY);
    sprintf(buffer, "%d.%d C", 20, 0);
    ILI9488_DrawString(box3_x1 + box_padding, box3_y1 + 60, 2.5, buffer, WHITE, GRAY);

    // Schedule entry 4 (right box)
    uint16_t box4_x1 = box3_x2 + box_spacing;
    uint16_t box4_x2 = box4_x1 + box_width;
    uint16_t box4_y1 = row2_y;
    uint16_t box4_y2 = box4_y1 + box_height;
    ILI9488_FilledRectangle(box4_x1, box4_y1, box4_x2, box4_y2, GRAY);
    ILI9488_DrawString(box4_x1 + box_padding, box4_y1 + 20, 2.5, "22:00", WHITE, GRAY);
    ILI9488_DrawString(box4_x1 + box_padding, box4_y1 + 40, 2.5, "->", LIGHT_GRAY, GRAY);
    sprintf(buffer, "%d.%d C", 18, 5);
    ILI9488_DrawString(box4_x1 + box_padding, box4_y1 + 60, 2.5, buffer, WHITE, GRAY);

    // ===== BOTTOM BUTTON ROW =====
    // Two buttons at the bottom with proper spacing (matching temperature screen style)
    const uint16_t BOTTOM_BUTTON_WIDTH = 200;
    const uint16_t TOTAL_BUTTON_WIDTH = (BOTTOM_BUTTON_WIDTH * 2) + BUTTON_SPACING;
    const uint16_t BOTTOM_START_X = (SCREEN_WIDTH - TOTAL_BUTTON_WIDTH) / 2;

    // Back button (left)
    uint16_t back_btn_x1 = BOTTOM_START_X;
    uint16_t back_btn_x2 = back_btn_x1 + BOTTOM_BUTTON_WIDTH;
    Draw_Modern_Button(back_btn_x1, BOTTOM_BUTTON_Y, back_btn_x2, BOTTOM_BUTTON_Y + BOTTOM_BUTTON_HEIGHT,
                       "Back", GRAY, WHITE, LIGHT_GRAY, 3);

    // Add New button (right)
    uint16_t add_btn_x1 = back_btn_x2 + BUTTON_SPACING;
    uint16_t add_btn_x2 = add_btn_x1 + BOTTOM_BUTTON_WIDTH;
    Draw_Modern_Button(add_btn_x1, BOTTOM_BUTTON_Y, add_btn_x2, BOTTOM_BUTTON_Y + BOTTOM_BUTTON_HEIGHT,
                       "Add New", GRAY, WHITE, LIGHT_GRAY, 3);
}

/**
 * @brief Draws the "Add Schedule" screen for creating a new entry
 */
void Plot_Touchscreen_AddSchedule_Screen(uint16_t year, uint8_t month, uint8_t day,
                                         uint8_t hour, uint8_t minute, uint8_t second)
{
    char buffer[50];

    // Screen dimensions (matching other screens)
    const uint16_t SCREEN_WIDTH = 480;
    const uint16_t SCREEN_HEIGHT = 320;
    const uint16_t MARGIN = 15;
    const uint16_t HEADER_HEIGHT = 50;
    const uint16_t BUTTON_SPACING = 15;
    const uint16_t BOTTOM_BUTTON_HEIGHT = 45;
    const uint16_t BOTTOM_BUTTON_Y = SCREEN_HEIGHT - BOTTOM_BUTTON_HEIGHT - MARGIN;
    const uint16_t CARD_SPACING = 12;
    const uint16_t CARD_HEIGHT = 50;
    const uint16_t NOTES_HEIGHT = 60;

    // Clear screen with dark blue background
    ILI9488_FillScreen(NAVY_BLUE);

    // ===== HEADER =====
    // Title bar with precise height (matching other screens)
    ILI9488_FilledRectangle(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, DARK_BLUE);
    ILI9488_DrawString(MARGIN, 8, 2, "Add Schedule Entry", WHITE, DARK_BLUE);

    // Date display (left side of header)
    sprintf(buffer, "%04d-%02d-%02d", year, month, day);
    ILI9488_DrawString(MARGIN, 30, 2, buffer, LIGHT_GRAY, DARK_BLUE);

    // Time display (right side of header)
    sprintf(buffer, "%02d:%02d:%02d", hour, minute, second);
    uint16_t time_x = SCREEN_WIDTH - MARGIN - (strlen(buffer) * 6 * 2);
    ILI9488_DrawString(time_x, 30, 2, buffer, WHITE, DARK_BLUE);

    // ===== INPUT CARDS =====
    // Calculate available space for cards
    uint16_t cards_start_y = HEADER_HEIGHT + MARGIN;
    uint16_t cards_end_y = BOTTOM_BUTTON_Y - MARGIN;
    uint16_t available_height = cards_end_y - cards_start_y;
    uint16_t total_cards_height = (CARD_HEIGHT * 2) + NOTES_HEIGHT + (CARD_SPACING * 2);
    uint16_t cards_y = cards_start_y + (available_height - total_cards_height) / 2; // Center cards vertically

    uint16_t card_x1 = MARGIN;
    uint16_t card_x2 = SCREEN_WIDTH - MARGIN;
    uint16_t card_text_y_offset = (CARD_HEIGHT / 2) - 8; // Center text vertically in card

    // Time card
    uint16_t time_card_y = cards_y;
    ILI9488_FilledRectangle(card_x1, time_card_y, card_x2, time_card_y + CARD_HEIGHT, GRAY);
    ILI9488_DrawString(card_x1 + 15, time_card_y + card_text_y_offset, 2, "Time", WHITE, GRAY);
    ILI9488_DrawString(card_x2 - 100, time_card_y + card_text_y_offset, 2, "HH:MM", LIGHT_GRAY, GRAY);

    // Temperature card
    uint16_t temp_card_y = time_card_y + CARD_HEIGHT + CARD_SPACING;
    ILI9488_FilledRectangle(card_x1, temp_card_y, card_x2, temp_card_y + CARD_HEIGHT, GRAY);
    ILI9488_DrawString(card_x1 + 15, temp_card_y + card_text_y_offset, 2, "Temperature", WHITE, GRAY);
    ILI9488_DrawString(card_x2 - 50, temp_card_y + card_text_y_offset, 2, "C", LIGHT_GRAY, GRAY);

    // Notes card
    uint16_t notes_card_y = temp_card_y + CARD_HEIGHT + CARD_SPACING;
    ILI9488_FilledRectangle(card_x1, notes_card_y, card_x2, notes_card_y + NOTES_HEIGHT, GRAY);
    ILI9488_DrawString(card_x1 + 15, notes_card_y + 12, 2, "Notes (Optional)", WHITE, GRAY);
    ILI9488_DrawString(card_x1 + 15, notes_card_y + 35, 2, "Tap to add notes", LIGHT_GRAY, GRAY);

    // ===== BOTTOM BUTTONS =====
    // Two buttons at the bottom with proper spacing (matching other screens)
    const uint16_t BOTTOM_BUTTON_WIDTH = 200;
    const uint16_t TOTAL_BUTTON_WIDTH = (BOTTOM_BUTTON_WIDTH * 2) + BUTTON_SPACING;
    const uint16_t BOTTOM_START_X = (SCREEN_WIDTH - TOTAL_BUTTON_WIDTH) / 2;

    // Cancel button (left)
    uint16_t cancel_btn_x1 = BOTTOM_START_X;
    uint16_t cancel_btn_x2 = cancel_btn_x1 + BOTTOM_BUTTON_WIDTH;
    Draw_Modern_Button(cancel_btn_x1, BOTTOM_BUTTON_Y, cancel_btn_x2, BOTTOM_BUTTON_Y + BOTTOM_BUTTON_HEIGHT,
                       "Cancel", GRAY, WHITE, LIGHT_GRAY, 3);

    // Save button (right)
    uint16_t save_btn_x1 = cancel_btn_x2 + BUTTON_SPACING;
    uint16_t save_btn_x2 = save_btn_x1 + BOTTOM_BUTTON_WIDTH;
    Draw_Modern_Button(save_btn_x1, BOTTOM_BUTTON_Y, save_btn_x2, BOTTOM_BUTTON_Y + BOTTOM_BUTTON_HEIGHT,
                       "Save", GRAY, WHITE, LIGHT_GRAY, 3);
}
