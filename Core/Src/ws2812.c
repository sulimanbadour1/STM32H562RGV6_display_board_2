/*
 * ws2812.c
 *
 *  Created on: Apr 26, 2025
 *      Author: Martin
 */

#include "ws2812.h"

#include "stm32h5xx_hal.h"
#include <stdlib.h>

#include "main.h"
#include "global_variables.h"

// this timing is set with oscilloscope
/*
#define T0H_NS  1   // 570 ns - this is not precise
#define T0L_NS  320   // 900 ns

#define T1H_NS  350   // 880 ns
#define T1L_NS  320   // 910 ns
*/

//#define RESET_US 500  // >280 us	//300

extern TIM_HandleTypeDef htim3;  // TIM2 must be initialized in CubeMX


/*
// Convert nanoseconds to timer ticks (250MHz = 4ns per tick)
static inline uint32_t ns_to_ticks(uint32_t ns) {
    return (uint32_t)((ns + 3) / 4); // Round up for safety
}
*/


/*
// Delay using TIM2 in one-pulse mode
void delay_ticks(uint32_t ticks) {
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_SET_AUTORELOAD(&htim2, ticks);
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start(&htim2);
    while (!__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE));
    HAL_TIM_Base_Stop(&htim2);
}
*/

/*
void delay_ticks(uint16_t ticks)
{
    TIM2->CNT = 0;
    TIM2->ARR = ticks;
    TIM2->EGR = TIM_EGR_UG;              // ensure ARR is updated now. Does two things:  Updates registers immediately: forces ARR/PSC values to be copied from preload.   Triggers an update event, setting flags and aligning counter logic.
    TIM2->SR = 0;                     // Clear update flag
    TIM2->CR1 |= TIM_CR1_CEN;        // Start timer
    while ((TIM2->SR & TIM_SR_UIF) == 0); // Wait for update
    TIM2->CR1 &= ~TIM_CR1_CEN;       // Stop timer
}



void send_bit(uint8_t bit)
{
    if (bit)
    {
        // Logic 1: High for T1H, then Low for T1L
        HAL_GPIO_WritePin(LED_RGB_DATA_GPIO_Port, LED_RGB_DATA_Pin, GPIO_PIN_SET);
        delay_ticks(ns_to_ticks(T1H_NS));
        HAL_GPIO_WritePin(LED_RGB_DATA_GPIO_Port, LED_RGB_DATA_Pin, GPIO_PIN_RESET);
        delay_ticks(ns_to_ticks(T1L_NS));
    } else
    {
        // Logic 0: High for T0H, then Low for T0L
        HAL_GPIO_WritePin(LED_RGB_DATA_GPIO_Port, LED_RGB_DATA_Pin, GPIO_PIN_SET);
        delay_ticks(ns_to_ticks(T0H_NS));
        HAL_GPIO_WritePin(LED_RGB_DATA_GPIO_Port, LED_RGB_DATA_Pin, GPIO_PIN_RESET);
        delay_ticks(ns_to_ticks(T0L_NS));
    }
}

void send_byte(uint8_t byte)
{
    for (int i = 7; i >= 0; i--)
    {
        send_bit((byte >> i) & 0x01);
    }
}

void delay_us(uint32_t us)
{
    delay_ticks(us * 250); // 250MHz → 250 ticks per microsecond
}


void ws2812_init(void)
{
    // GPIO PB0 as output push-pull
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LED_RGB_DATA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED_RGB_DATA_GPIO_Port, &GPIO_InitStruct);

    // TIM2 setup assumed via CubeMX, or add manually:
    __HAL_RCC_TIM2_CLK_ENABLE();
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFF;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim2);

    HAL_TIM_Base_Start(&htim2);

}
*/

// WS2812 uses GRB order
// function for 1 LED
/*
void ws2812_send_color(uint8_t r, uint8_t g, uint8_t b)
{
    send_byte(g);
    send_byte(r);
    send_byte(b);
    // Reset pulse
    HAL_GPIO_WritePin(LED_RGB_DATA_GPIO_Port, LED_RGB_DATA_Pin, GPIO_PIN_RESET);
    delay_us(500); // 500us reset pulse
}

void ws2812_send_random_color(void)
{
    uint8_t r = rand() % 256;
    uint8_t g = rand() % 256;
    uint8_t b = rand() % 256;
    ws2812_send_color(r, g, b);
}

*/


// function for 3 LEDs
/*
void ws2812_send_colors(uint8_t (*colors)[3], uint8_t count)
{
    for (uint8_t i = 0; i < count; i++)
    {
        uint8_t g = colors[i][1];
        uint8_t r = colors[i][0];
        uint8_t b = colors[i][2];

        send_byte(g);
        send_byte(r);
        send_byte(b);
    }

    // Send reset signal (low for >280us)
    HAL_GPIO_WritePin(LED_RGB_DATA_GPIO_Port, LED_RGB_DATA_Pin, GPIO_PIN_RESET);
    delay_us(300);  // 300 us
}
*/


void WS2812_send_random_colors_with_brightness(uint8_t brightness)
{
/*
	for (uint8_t i = 0; i < count; i++)
    {
        uint8_t r = (colors[i][0] * brightness) / 255;
        uint8_t g = (colors[i][1] * brightness) / 255;
        uint8_t b = (colors[i][2] * brightness) / 255;

        send_byte(g);  // GRB order
        send_byte(r);
        send_byte(b);
    }

    HAL_GPIO_WritePin(LED_RGB_DATA_GPIO_Port, LED_RGB_DATA_Pin, GPIO_PIN_RESET);
    delay_us(300);
*/

    for (int i = 0; i < WS2812_LED_COUNT; i++)
    {
    	uint8_t R = ( (rand() % 256) * brightness) / 255;
    	uint8_t G = ( (rand() % 256) * brightness) / 255;
    	uint8_t B = ( (rand() % 256) * brightness) / 255;

        LED_colors[i] = ((G <<16) | (R<<8) | B) ;
	}

    WS2812_Send(LED_colors);

}

void WS2812_send_random_colors(void)
{
/*
	uint8_t colors[3][3]; // 3 LEDs, RGB each

    for (int i = 0; i < 3; i++)
    {
        colors[i][0] = rand() % 256; // R
        colors[i][1] = rand() % 256; // G
        colors[i][2] = rand() % 256; // B
    }

    ws2812_send_colors(colors, 3);
*/

    for (int i = 0; i < WS2812_LED_COUNT; i++)
    {
        LED_colors[i] = (((rand() % 256)<<16) | ((rand() % 256)<<8) | (rand() % 256));
	}

    WS2812_Send(LED_colors);
}



void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_3);
}


uint16_t pwmData[(24*WS2812_LED_COUNT)+50];

void WS2812_Send (uint32_t *color)
{
	uint32_t indx = 0;

	// create pwmData to be send later with DMA
	// loop for all LEDs
	for (int j= 0; j<WS2812_LED_COUNT; j++)
	{
		for (int i=23; i>=0; i--)
		{
			if (color[j]&(1<<i))
			{
				pwmData[indx] = 70;  // 66 = 790ns, 70 = 835ns
			}
			else
			{
				pwmData[indx] = 25;  // 33 = 390ns, 25 = 300ns
			}
				indx++;
		}
	}

	// add pause between data
	//After storing all the values for the LEDs, we need to also store the values, to keep the Pulse LOW for more than 50 us
	//To achieve this, we can store 50 zeroes. Since our period is 1.25 us, 50 zeroes will cover more than 50 us of the LOW time
	for (int i=0; i<50; i++)
	{
		pwmData[indx] = 0;
		indx++;
	}

	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3, (uint32_t *)pwmData, sizeof(pwmData));
	// now wait until all data is send with DMA
	// when finished  HAL_TIM_PWM_PulseFinishedCallback
	// is called to stop PWM generation
}


void WS2812_send_colors_with_brightness (uint32_t *color, uint8_t brightness)
{
	uint32_t indx = 0;

	uint32_t temp_color = 0;

	// create pwmData to be send later with DMA
	// loop for all LEDs
	for (int j= 0; j<WS2812_LED_COUNT; j++)
	{
		temp_color = (color[j] * brightness) >> 8;

		for (int i=23; i>=0; i--)
		{
			if (temp_color&(1<<i))
			{
				pwmData[indx] = 70;  // 66 = 790ns, 70 = 835ns
			}
			else
			{
				pwmData[indx] = 25;  // 33 = 390ns, 25 = 300ns
			}
				indx++;
		}
	}

	// add pause between data
	//After storing all the values for the LEDs, we need to also store the values, to keep the Pulse LOW for more than 50 us
	//To achieve this, we can store 50 zeroes. Since our period is 1.25 us, 50 zeroes will cover more than 50 us of the LOW time
	for (int i=0; i<50; i++)
	{
		pwmData[indx] = 0;
		indx++;
	}

	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3, (uint32_t *)pwmData, sizeof(pwmData));
	// now wait until all data is send with DMA
	// when finished  HAL_TIM_PWM_PulseFinishedCallback
	// is called to stop PWM generation
}
