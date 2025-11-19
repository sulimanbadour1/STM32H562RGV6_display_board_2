/*
 * global_variables.h
 *
 *  Created on: May 24, 2025
 *      Author: Martin
 */

#ifndef INC_GLOBAL_VARIABLES_H_
#define INC_GLOBAL_VARIABLES_H_

	// if SWV_ITM_Debug 1
	// printf can be seen in SWV_ITM_Debug Data Console
	#define SWV_ITM_Debug 1

	// if PERIPHERAL_DEBUG 1
	// used for initial step by step testing of all on board peripherals
	//#define PERIPHERAL_DEBUG 1


	// if LCD_IS_MODBUS_MASTER 1
	// the LCD acts as MODBUS master and periodically reads data from slave, e.g. BMS
	#define LCD_IS_MODBUS_MASTER 1

	// if LCD_IS_MODBUS_SLAVE 1
	// the LCD acts as MODBUS slave, data update is done from the master, e.g. BMS
	//#define LCD_IS_MODBUS_SLAVE 1

	// LCD_IS_MODBUS_MASTER and LCD_IS_MODBUS_SLAVE CANT be both defined at the same time

	#define LCD_ORIENTATION_LANDSCAPE 1
	//#define LCD_ORIENTATION_PORTRAIT 1

	// Structure declaration
	// this structure will store all values obtained from the BQ76972 Analog Front End
	struct BMS_data_structure
	{
		uint16_t SOC;						// state of charge, in 0.1 percent
		int16_t Power_W;					// BMS power in W

		int16_t Internal_Temperature_K;		// This is the most recent measured internal die temperature, in 0.1 K
		int16_t Internal_Temperature_C;		// This is the most recent measured internal die temperature, in 0.1 C
		int16_t current_userA;				// 16-bit CC2 current, in 0.1A
		uint16_t Stack_Voltage_mV; 			// 16-bit voltage on top of stack, in mV

		uint16_t Cell_1_voltage_mV;			// 16-bit voltage on cell 1, in mV
		uint16_t Cell_2_voltage_mV;			// 16-bit voltage on cell 2, in mV
		uint16_t Cell_3_voltage_mV;			// 16-bit voltage on cell 3, in mV
		uint16_t Cell_4_voltage_mV;			// 16-bit voltage on cell 4, in mV
		uint16_t Cell_5_voltage_mV;			// 16-bit voltage on cell 5, in mV
		uint16_t Cell_6_voltage_mV;			// 16-bit voltage on cell 6, in mV
		uint16_t Cell_7_voltage_mV;			// 16-bit voltage on cell 7, in mV
		uint16_t Cell_8_voltage_mV;			// 16-bit voltage on cell 8, in mV
		uint16_t Cell_9_voltage_mV;			// 16-bit voltage on cell 9, in mV
		uint16_t Cell_10_voltage_mV;		// 16-bit voltage on cell 10, in mV
		uint16_t Cell_11_voltage_mV;		// 16-bit voltage on cell 11, in mV
		uint16_t Cell_12_voltage_mV;		// 16-bit voltage on cell 12, in mV
		uint16_t Cell_13_voltage_mV;		// 16-bit voltage on cell 13, in mV
		uint16_t Cell_14_voltage_mV;		// 16-bit voltage on cell 14, in mV
		uint16_t Cell_15_voltage_mV;		// 16-bit voltage on cell 15, in mV
		uint16_t Cell_16_voltage_mV;		// 16-bit voltage on cell 16, in mV

	};

	// variable with structure that holds all BMS info
	// defined only once i main.c
	extern struct BMS_data_structure BMS_no1;

	#define WS2812_LED_COUNT 3
	extern uint32_t LED_colors[WS2812_LED_COUNT];

#define UART_RX_BUFFER_SIZE  256
#define MODBUS_REGISTERS_SIZE  256

#endif /* INC_GLOBAL_VARIABLES_H_ */
