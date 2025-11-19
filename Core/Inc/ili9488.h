
#ifndef INC_ILI9488_H_
#define INC_ILI9488_H_

#include "main.h"

// Assign human-readable names to some common 16-bit color values:
//RGB565 Color Picker
//https://rgbcolorpicker.com/565
#define BLACK   		0x0000
#define BLUE   			0x001F
#define RED     		0xF800
#define GREEN   		0x07E0
#define WHITE   		0xFFFF
#define MAGENTA 		0xF81F
#define YELLOW  		0xFFE0
#define CYAN    		0x1FFF
#define GRAY    		0x8410

#define DARKER_GREEN    0x1CA7
#define LIGHTER_GREEN   0x8793
#define NAVY_BLUE      0x000F   // Very dark blue
#define DARK_BLUE      0x0013   // Darker than BLUE but brighter than NAVY
#define LIGHT_BLUE     0x7D7C   // Soft light blue (close to sky blue)
#define LIGHT_GRAY     0xC618   // Standard RGB565 light gray
#define ORANGE         0xFD20   // Strong orange (commonly used in TFT libs)
#define DARK_GRAY      0x4208   // Dark gray


void ILI9488_Init(void);
//void ILI9488_WriteCommand(uint8_t cmd);
//void ILI9488_WriteData(uint8_t data);
//static inline void ILI9488_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ILI9488_FillScreen(uint16_t color);

void ILI9488_FilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t color);
void ILI9488_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ILI9488_DrawChar(uint16_t x, uint16_t y, uint8_t scale, char ch, uint16_t color, uint16_t bg_color);
void ILI9488_DrawString(uint16_t x, uint16_t y, uint8_t scale, char *str, uint16_t color, uint16_t bgcolor);
void ILI9488_DrawButton(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t scale, char *str, uint16_t color, uint16_t bgcolor);

// lines
void ILI9488_V_line(unsigned int x, unsigned int y, unsigned int length, unsigned int color);
void ILI9488_H_line(unsigned int x, unsigned int y, unsigned int length, unsigned int color);
void ILI9488_drawThickLine(int x0, int y0, int x1, int y1, uint8_t thickness, uint16_t color);

// geometrical shapes -> rectangles, circles etc.
void ILI9488_Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int color);
void ILI9488_Rect_3(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int color);
void ILI9488_drawCircleOutline(int x0, int y0, int radius, uint8_t thickness, uint16_t color);

// other symbols
void ILI9488_DrawBatterySymbol(uint16_t x, uint16_t y, uint16_t size, uint16_t color);


#endif /* INC_ILI9488_H_ */
