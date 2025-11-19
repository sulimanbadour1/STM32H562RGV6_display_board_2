/*
 * modbus.h
 *
 *  Created on: May 26, 2025
 *      Author: Martin
 */

#ifndef MODBUS_H
#define MODBUS_H

#include <stdint.h>
#include "stm32h5xx_hal.h"  // Change based on MCU

// Modbus Function Codes
#define MODBUS_READ_HOLDING_REGISTERS  0x03

// UART handle declaration (extern for global access)
extern UART_HandleTypeDef huart5;

// Function prototypes
void Modbus_Request_ReadRegisters(uint16_t start_address, uint16_t num_registers);

uint16_t Modbus_Calculate_CRC(uint8_t *data, uint16_t length);
void Modbus_Send_Response(uint8_t *request, uint16_t *data, uint16_t count);
void Modbus_Process_Request(uint8_t *frame, uint16_t length);
void Modbus_Response_Handler(uint8_t *response, uint16_t length);

#endif
