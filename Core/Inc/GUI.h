/*
 * GUI.h
 *
 *  Created on: May 31, 2025
 *      Author: Martin
 */

#ifndef INC_GUI_H_
#define INC_GUI_H_

	void Plot_GUI_screen_1();
	void Plot_GUI_screen_2();
	void Plot_GUI_screen_3();
	void Plot_GUI_screen_4();

	// Touchscreen UI screens
	// Date/time structure: year, month, day, hour, minute, second
	void Plot_Touchscreen_Temperature_Screen(int16_t temperature,
	                                          uint16_t year, uint8_t month, uint8_t day,
	                                          uint8_t hour, uint8_t minute, uint8_t second);
	void Plot_Touchscreen_Schedule_Screen(uint16_t year, uint8_t month, uint8_t day,
	                                      uint8_t hour, uint8_t minute, uint8_t second);
	void Plot_Touchscreen_AddSchedule_Screen(uint16_t year, uint8_t month, uint8_t day,
	                                          uint8_t hour, uint8_t minute, uint8_t second);
	void Plot_Touchscreen_ScheduleWizard_Day(uint16_t year, uint8_t month, uint8_t day,
	                                         uint8_t hour, uint8_t minute, uint8_t second,
	                                         uint8_t selected_weekday);
	void Plot_Touchscreen_ScheduleWizard_Time(uint16_t year, uint8_t month, uint8_t day,
	                                          uint8_t hour, uint8_t minute, uint8_t second,
	                                          uint8_t selected_weekday, uint8_t target_hour, uint8_t target_minute);
	void Plot_Touchscreen_ScheduleWizard_Temperature(uint16_t year, uint8_t month, uint8_t day,
	                                                  uint8_t hour, uint8_t minute, uint8_t second,
	                                                  int16_t target_temperature_tenths,
	                                                  uint8_t selected_weekday, uint8_t target_hour, uint8_t target_minute);
	void Plot_Touchscreen_ScheduleWizard_Name(uint16_t year, uint8_t month, uint8_t day,
	                                          uint8_t hour, uint8_t minute, uint8_t second,
	                                          const char *entry_name, int16_t target_temperature_tenths,
	                                          uint8_t selected_weekday, uint8_t target_hour, uint8_t target_minute);

#endif /* INC_GUI_H_ */
