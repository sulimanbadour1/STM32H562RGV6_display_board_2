/*
 * ws2812.h
 *
 *  Created on: Apr 26, 2025
 *      Author: Martin
 */

#ifndef INC_WS2812_H_
#define INC_WS2812_H_

	#include "stm32h5xx_hal.h"

	// Configuration

	//#define WS2812_TIMER htim3

	// define color names
	// WS2812 uses GRB color order (Green, Red, Blue)
	#define WS2812_BLUE     0x0000FF  // G=0, R=0, B=255
	#define WS2812_RED      0x00FF00  // G=0, R=255, B=0
	#define WS2812_GREEN    0xFF0000  // G=255, R=0, B=0
	#define WS2812_WHITE    0xFFFFFF  // G=255, R=255, B=255
	#define WS2812_MAGENTA  0x00FFEF  // G=0, R=255, B=255 (R+B)
	#define WS2812_YELLOW   0xFFFF00  // G=255, R=255, B=0 (R+G)
	#define WS2812_CYAN     0xFF00FF  // G=255, R=0, B=255 (G+B)
	#define WS2812_GRAY     0x7F7F7F  // G=127, R=127, B=127 (medium gray)


	//void ws2812_init(void);
	//void ws2812_send_color(uint8_t r, uint8_t g, uint8_t b);
	//void ws2812_send_random_color(void);
	void WS2812_Send (uint32_t *color);
	void WS2812_send_colors_with_brightness (uint32_t *color, uint8_t brightness);
	//void ws2812_send_colors(uint8_t (*colors)[3], uint8_t count);
	void WS2812_send_random_colors(void);
	void WS2812_send_random_colors_with_brightness(uint8_t brightness);

#endif /* INC_WS2812_H_ */
