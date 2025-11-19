/*
 * modbus.c
 *
 *  Created on: May 26, 2025
 *      Author: Martin
 */


#include "stm32h5xx_hal.h"
#include <stdio.h>

#include "modbus.h"
#include "main.h"
#include "global_variables.h"


#define SLAVE_ADDRESS  0x01					// Modbus slave address

uint8_t request_frame[8];					// TX buffer

extern UART_HandleTypeDef huart5;						// uart5 is user for MODBUS communication
extern TIM_HandleTypeDef htim6;							// TIM6 is used for MODBUS receive timeout to detect IDLE line

extern uint8_t byte;
extern uint8_t response_buffer[UART_RX_BUFFER_SIZE];
extern uint16_t response_index;
extern uint8_t modbus_receiving;


extern uint16_t modbus_registers[UART_RX_BUFFER_SIZE];  		// Simulate up to 125 holding registers (max per Modbus spec)



/**
  * @brief  Starts Modbus RTU reception
  *
  * @param  none
  * @retval none
  */
void Start_Modbus_Reception(void)
{
    response_index = 0;
    modbus_receiving = 1;
    HAL_UART_Receive_IT(&huart5, &byte, 1);
}


/**
  * @brief  Calculates CRC16 checksum for Modbus RTU.
  *
  * @param  data: pointer to the input data buffer
  * @param  length: number of bytes in the buffer
  * @retval 16-bit CRC value
  */
uint16_t Modbus_Calculate_CRC(uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t pos = 0; pos < length; pos++)
    {
        crc ^= (uint16_t)data[pos];
        for (uint8_t i = 0; i < 8; i++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            } else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}


/**
  * @brief  Sends a Modbus RTU request to read holding registers.
  *
  * @note   This function constructs the request frame and sends it over UART.
  *         RS485 direction pin is toggled for transmit/receive.
  *
  * @param  start_address: starting register address to read
  * @param  num_registers: number of registers to read (1 to 125)
  * @retval None
  */
void Modbus_Request_ReadRegisters(uint16_t start_address, uint16_t num_registers)
{
	// create frame request
    request_frame[0] = SLAVE_ADDRESS;
    request_frame[1] = 0x03;							// Function code: Read Holding Registers
    request_frame[2] = (start_address >> 8) & 0xFF;
    request_frame[3] = start_address & 0xFF;
    request_frame[4] = (num_registers >> 8) & 0xFF;
    request_frame[5] = num_registers & 0xFF;

    uint16_t crc = Modbus_Calculate_CRC(request_frame, 6);
    request_frame[6] = crc & 0xFF;
    request_frame[7] = (crc >> 8) & 0xFF;

    // RS485_1_DIR to transmit
	HAL_GPIO_WritePin (RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_SET);
    // send request
    //HAL_UART_Transmit(&huart5, request_frame, 8, HAL_MAX_DELAY);	// this is blocking
	// RS485_1_DIR back to receive
	// HAL_GPIO_WritePin (RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET);

	HAL_UART_Transmit_DMA(&huart5, request_frame, 8);				// this uses DMA for transmit

}


/**
  * @brief  Parses and validates received Modbus response.
  *
  * @param  response: pointer to response buffer
  * @param  length: length of received data
  * @retval None
  */
void Modbus_Response_Handler(uint8_t *response, uint16_t length)
{
    if (length < 5) return; // Minimum length: address + function + byte count + 2 CRC

    uint16_t received_crc = (response[length - 1] << 8) | response[length - 2];
    uint16_t calculated_crc = Modbus_Calculate_CRC(response, length - 2);

    if (received_crc != calculated_crc)
    {
        printf("CRC error, discarding response\n");
        return;
    }

    if (response[1] != 0x03)
    {
        printf("Unexpected function code\n");
        return;
    }

    uint8_t byte_count = response[2];
    if (byte_count > 250 || (byte_count % 2 != 0))
    {
        printf("Invalid byte count\n");
        return;
    }

    // process response and store to registers
    uint8_t num_registers = byte_count / 2;
    for (uint8_t i = 0; i < num_registers; i++)
    {
        modbus_registers[i] = (response[3 + i * 2] << 8) | response[4 + i * 2];
    }
    response_index = 0;
    printf("Received %d registers from slave\n", num_registers);

    // copy modbus_registers to BMS_data_structure
    BMS_no1.SOC = modbus_registers[0];								// state of charge, in 0.1 percent
    BMS_no1.Power_W = modbus_registers[1];							// BMS power in W
    BMS_no1.Internal_Temperature_K = modbus_registers[2];			// This is the most recent measured internal die temperature, in 0.1 K
	BMS_no1.Internal_Temperature_C = modbus_registers[3];			// This is the most recent measured internal die temperature, in 0.1 C
	BMS_no1.current_userA = modbus_registers[4];					// 16-bit CC2 current, in 0.1A
	BMS_no1.Stack_Voltage_mV = modbus_registers[5];					// 16-bit voltage on top of stack, in mV
	BMS_no1.Cell_1_voltage_mV = modbus_registers[6];				// 16-bit voltage on cell 1, in mV
	BMS_no1.Cell_2_voltage_mV = modbus_registers[7];				// 16-bit voltage on cell 2, in mV
	BMS_no1.Cell_3_voltage_mV = modbus_registers[8];				// 16-bit voltage on cell 3, in mV
	BMS_no1.Cell_4_voltage_mV = modbus_registers[9];				// 16-bit voltage on cell 4, in mV
	BMS_no1.Cell_5_voltage_mV = modbus_registers[10];				// 16-bit voltage on cell 5, in mV
	BMS_no1.Cell_6_voltage_mV = modbus_registers[11];				// 16-bit voltage on cell 6, in mV
	BMS_no1.Cell_7_voltage_mV = modbus_registers[12];				// 16-bit voltage on cell 7, in mV
	BMS_no1.Cell_8_voltage_mV = modbus_registers[13];				// 16-bit voltage on cell 8, in mV
	BMS_no1.Cell_9_voltage_mV = modbus_registers[14];				// 16-bit voltage on cell 9, in mV
	BMS_no1.Cell_10_voltage_mV = modbus_registers[15];				// 16-bit voltage on cell 10, in mV
	BMS_no1.Cell_11_voltage_mV = modbus_registers[16];				// 16-bit voltage on cell 11, in mV
	BMS_no1.Cell_12_voltage_mV = modbus_registers[17];				// 16-bit voltage on cell 12, in mV
	BMS_no1.Cell_13_voltage_mV = modbus_registers[18];				// 16-bit voltage on cell 13, in mV
	BMS_no1.Cell_14_voltage_mV = modbus_registers[19];				// 16-bit voltage on cell 14, in mV
	BMS_no1.Cell_15_voltage_mV = modbus_registers[20];				// 16-bit voltage on cell 15, in mV
	BMS_no1.Cell_16_voltage_mV = modbus_registers[20];				// 16-bit voltage on cell 16, in mV

}


// prevent compiler to optimize this code, this is a delay loop
#pragma GCC push_options
#pragma GCC optimize ("O0")
void some_delay(void)
{
    for (uint32_t i = 0; i < 5000; i++)
    {
    	__NOP(); __NOP(); __NOP(); __NOP();
    }

}
#pragma GCC pop_options

/**
  * @brief  UART DMA complete callback.
  *
  * @note   Called automatically when UART finishes DMA transmission.
  *         When transmission is complete RS485_1_DIR back to receive
  *
  * @param  huart: UART handle
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
// based on https://blog.embeddedexpert.io/?p=2881

	if(huart->Instance==UART5)
	{
		// when transmission is complete
		// RS485_1_DIR back to receive
		HAL_GPIO_WritePin (RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET);

	    // add a small delay to wait for  RS485_1_DIR to switch
		some_delay();

		Start_Modbus_Reception();  // Start reception after request is sent
	}
}



/**
  * @brief  UART receive complete callback.
  *
  * @note   Called automatically when UART byte is received
  *         Stores received byte in buffer and restarts to receive next byte
  *
  * @param  huart: UART handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART5 && modbus_receiving)
    {
        if (response_index < sizeof(response_buffer))
        {
            response_buffer[response_index++] = byte;

            // prevent buffer overflow
            if (response_index > sizeof(response_buffer))
            {
            	response_index = 0;
            }

            // Restart byte reception
            HAL_UART_Receive_IT(&huart5, &byte, 1);

            // Restart response timeout timer (e.g. TIM6)
            // timeout detects IDLE line
            __HAL_TIM_SET_COUNTER(&htim6, 0);
            HAL_TIM_Base_Start_IT(&htim6);
        }
    }
}



/*
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	uint16_t received_data_length = Size;

    if (huart->Instance == UART5)
    {
    	// Minimum length: address + function + byte count + 2 CRC
    	if (received_data_length >= 5)
    	{
            // Try to parse Modbus response
            Modbus_Response_Handler(response_buffer, received_data_length);

            // Restart DMA for future frames (optional)
            //HAL_UART_Receive_DMA(&huart5, response_buffer, 256);
    	}

        // Restart reception regardless
        //HAL_UARTEx_ReceiveToIdle_DMA(&huart5, response_buffer, sizeof(response_buffer));

    }
}
*/

/**
  * @brief  UART DMA complete callback.
  *
  * @note   Called automatically when UART finishes DMA reception.
  *         Processes the received Modbus frame.
  *
  * @param  huart: UART handle
  * @retval None
  */
/*
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//void HAL_UARTEx_RxCpltCallback(UART_HandleTypeDef *huart)
{
//based on https://blog.embeddedexpert.io/?p=2900
    if (huart->Instance == UART5)
    {
        // Try to parse Modbus response
        Modbus_Response_Handler(response_buffer, 256);

        // Restart DMA for future frames (optional)
        //HAL_UART_Receive_DMA(&huart5, response_buffer, 256);
    }
}
*/
