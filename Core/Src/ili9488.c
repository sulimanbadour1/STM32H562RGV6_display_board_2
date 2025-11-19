
#include "ili9488.h"


#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fonts.h"

#include "global_variables.h"

// Screen resolution
uint16_t ILI9488_WIDTH = 480;
uint16_t ILI9488_HEIGHT = 320;


#define LCD_RS_LOW()   (GPIOD->BSRR = GPIO_BSRR_BR12)
#define LCD_RS_HIGH()  (GPIOD->BSRR = GPIO_BSRR_BS12)
#define LCD_nRESET_LOW()   (GPIOA->BSRR = GPIO_BSRR_BR8)
#define LCD_nRESET_HIGH()  (GPIOA->BSRR = GPIO_BSRR_BS8)
#define LCD_RD_LOW()   (GPIOD->BSRR = GPIO_BSRR_BR11)
#define LCD_RD_HIGH()  (GPIOD->BSRR = GPIO_BSRR_BS11)
#define LCD_WR_LOW()   (GPIOB->BSRR = GPIO_BSRR_BR10)
#define LCD_WR_HIGH()  (GPIOB->BSRR = GPIO_BSRR_BS10)
#define LCD_nCS_LOW()   (GPIOA->BSRR = GPIO_BSRR_BR3)
#define LCD_nCS_HIGH()  (GPIOA->BSRR = GPIO_BSRR_BS3)


static inline void ILI9488_Write8(uint8_t data)
{
    // Toggle WR LOW
    LCD_WR_LOW();
	GPIOC->ODR = data;
    //__NOP();  // Delay to meet timing
    LCD_WR_HIGH();

}

static inline void ILI9488_Write16(uint16_t data)
{
    // Toggle WR LOW
    LCD_WR_LOW();
	GPIOC->ODR = data;
    //__NOP();  // Delay to meet timing
    LCD_WR_HIGH();
}



static inline void ILI9488_WriteCommand(uint8_t cmd)
{
    LCD_RS_LOW();
    ILI9488_Write8(cmd);
}

/*
// 8 bit version
static inline void ILI9488_WriteData(uint8_t data)
{
    LCD_RS_HIGH();
    ILI9488_Write8(data);
}
*/

// 16 bit version
static inline void ILI9488_WriteData(uint16_t data)
{
    LCD_RS_HIGH();
    ILI9488_Write16(data);
}

void ILI9488_Reset(void)
{
	LCD_nRESET_LOW();
    HAL_Delay(10);
    LCD_nRESET_HIGH();
    HAL_Delay(10);
}

void ILI9488_Init(void)
{
	// initial state of pins
	LCD_RD_HIGH();
	LCD_WR_HIGH();
	LCD_RS_HIGH();
	LCD_nCS_LOW();
	LCD_nRESET_LOW();

    HAL_Delay(5);
    LCD_nCS_LOW();
    ILI9488_Reset();

    ILI9488_WriteCommand(0xE0); // Positive Gamma Control
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x03);
    ILI9488_WriteData(0x09);
    ILI9488_WriteData(0x08);
    ILI9488_WriteData(0x16);
    ILI9488_WriteData(0x0A);
    ILI9488_WriteData(0x3F);
    ILI9488_WriteData(0x78);
    ILI9488_WriteData(0x4C);
    ILI9488_WriteData(0x09);
    ILI9488_WriteData(0x0A);
    ILI9488_WriteData(0x08);
    ILI9488_WriteData(0x16);
    ILI9488_WriteData(0x1A);
    ILI9488_WriteData(0x0F);

    ILI9488_WriteCommand(0xE1); // Negative Gamma Control
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x16);
    ILI9488_WriteData(0x19);
    ILI9488_WriteData(0x03);
    ILI9488_WriteData(0x0F);
    ILI9488_WriteData(0x05);
    ILI9488_WriteData(0x32);
    ILI9488_WriteData(0x45);
    ILI9488_WriteData(0x46);
    ILI9488_WriteData(0x04);
    ILI9488_WriteData(0x0E);
    ILI9488_WriteData(0x0D);
    ILI9488_WriteData(0x35);
    ILI9488_WriteData(0x37);
    ILI9488_WriteData(0x0F);

    ILI9488_WriteCommand(0xC0); // Power Control 1
    ILI9488_WriteData(0x17);
    ILI9488_WriteData(0x15);

    ILI9488_WriteCommand(0xC1); // Power Control 2
    ILI9488_WriteData(0x41);

    ILI9488_WriteCommand(0xC5); // VCOM Control
    ILI9488_WriteData(0x00);
    ILI9488_WriteData(0x12);
    ILI9488_WriteData(0x80);

    ILI9488_WriteCommand(0x36); // Memory Access Control
    ILI9488_WriteData(0x48);    // RGB

    ILI9488_WriteCommand(0x3A); // Pixel Format
    //ILI9488_WriteData(0x66);    // 18-bit/pixel
    ILI9488_WriteData(0x55);    // 16-bit/pixel

    ILI9488_WriteCommand(0x11); // Sleep Out
    HAL_Delay(120);

    ILI9488_WriteCommand(0x29); // Display ON

}

static inline void ILI9488_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{

    ILI9488_WriteCommand(0x2A); // Column Address Set
    ILI9488_WriteData(x0 >> 8);
    ILI9488_WriteData(x0 & 0xFF);
    ILI9488_WriteData(x1 >> 8);
    ILI9488_WriteData(x1 & 0xFF);

    ILI9488_WriteCommand(0x2B); // Row Address Set
    ILI9488_WriteData(y0 >> 8);
    ILI9488_WriteData(y0 & 0xFF);
    ILI9488_WriteData(y1 >> 8);
    ILI9488_WriteData(y1 & 0xFF);

    ILI9488_WriteCommand(0x2C); // Memory Write

}

/*
//8 bit version
void ILI9488_FillScreen(uint16_t color)
{
	ILI9488_SetAddressWindow(0, 0, 320, 480);

    //ILI9488_WriteCommand(0x2C); // Memory write
    for (uint32_t i = 0; i < 320UL * 480UL; i++)
    {
        ILI9488_WriteData(color >> 8);
        ILI9488_WriteData(color & 0xFF);
    }
}
*/

/*
//16 bit version
void ILI9488_FillScreen(uint16_t color)
{
	ILI9488_SetAddressWindow(0, 0, 320, 480);

    //ILI9488_WriteCommand(0x2C); // Memory write
    for (uint32_t i = 0; i < 320UL * 480UL; i++)
    {
        ILI9488_WriteData(color);
    }
}
*/

/*
void ILI9488_FillScreen(uint16_t color)
{
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    ILI9488_SetAddressWindow(0, 0, 319, 479);

    LCD_RS_HIGH();  // Data mode for memory write

    for (uint32_t i = 0; i < 320UL * 480UL; i++)
    {
        ILI9488_Write8(hi);
        ILI9488_Write8(lo);
    }
}
*/

/*
// function with unrolled loop for extra speed
// 8 bit version
__attribute__((optimize("O3")))
void ILI9488_FillScreen(uint16_t color)
{
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    ILI9488_SetAddressWindow(0, 0, ILI9488_HEIGHT-1, ILI9488_WIDTH-1);

    LCD_RS_HIGH();  // Data mode

    uint32_t pixels = ILI9488_HEIGHT * ILI9488_WIDTH;
    uint32_t iterations = pixels / 8;

    for (uint32_t i = 0; i < iterations; i++)
    {
        ILI9488_Write8(hi); ILI9488_Write8(lo);  // 1
        ILI9488_Write8(hi); ILI9488_Write8(lo);  // 2
        ILI9488_Write8(hi); ILI9488_Write8(lo);  // 3
        ILI9488_Write8(hi); ILI9488_Write8(lo);  // 4
        ILI9488_Write8(hi); ILI9488_Write8(lo);  // 5
        ILI9488_Write8(hi); ILI9488_Write8(lo);  // 6
        ILI9488_Write8(hi); ILI9488_Write8(lo);  // 7
        ILI9488_Write8(hi); ILI9488_Write8(lo);  // 8
    }
}

*/


// function with unrolled loop for extra speed
// 16 bit version
__attribute__((optimize("O3")))
void ILI9488_FillScreen(uint16_t color)
{
    ILI9488_SetAddressWindow(0, 0, ILI9488_HEIGHT-1, ILI9488_WIDTH-1);
    LCD_RS_HIGH();  // Data mode

    uint32_t pixels = ILI9488_HEIGHT * ILI9488_WIDTH;
    uint32_t iterations = pixels / 8;

    for (uint32_t i = 0; i < iterations; i++)
    {
        ILI9488_Write16(color);			// 1
        ILI9488_Write16(color);			// 2
        ILI9488_Write16(color);			// 3
        ILI9488_Write16(color);			// 4
        ILI9488_Write16(color);			// 5
        ILI9488_Write16(color);			// 6
        ILI9488_Write16(color);			// 7
        ILI9488_Write16(color);			// 8
    }
}


/*
// 8 bit version
void ILI9488_FilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t color)
{
	ILI9488_SetAddressWindow(x0, y0, x1, y1);

	uint16_t dx = x1 - x0;
	uint16_t dy = y1 - y0;
	uint16_t data_size = dx*dy;

    for (uint32_t i = 0; i < data_size; i++)
    {
        ILI9488_WriteData(color >> 8);
        ILI9488_WriteData(color & 0xFF);
    }
}
*/

// 16 bit version
void ILI9488_FilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t color)
{

#ifdef LCD_ORIENTATION_PORTRAIT
	ILI9488_SetAddressWindow(x0, y0, x1, y1);

	uint16_t dx = x1 - x0;
	uint16_t dy = y1 - y0;
	uint16_t data_size = dx*dy;

    for (uint32_t i = 0; i < data_size+dx/2; i++)
    {
        ILI9488_WriteData(color);
    }

#endif

#ifdef LCD_ORIENTATION_LANDSCAPE
	ILI9488_SetAddressWindow(y0, x0, y1, x1);

	uint16_t dx = x1 - x0;
	uint16_t dy = y1 - y0;
	uint16_t data_size = dx*dy;

    for (uint32_t i = 0; i < data_size+dy/2; i++)
    {
        ILI9488_WriteData(color);
    }

#endif
}

/*
// 8 bit version
void ILI9488_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	// check if not outside of display
    if ((y >= ILI9488_WIDTH) || (x >= ILI9488_HEIGHT)) return;

    ILI9488_SetAddressWindow(x, y, x, y);

    ILI9488_WriteData(color >> 8);
    ILI9488_WriteData(color & 0xFF);

}
*/

// 16 bit version
void ILI9488_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{

#ifdef LCD_ORIENTATION_LANDSCAPE
	// swap X and Y in this orientation
	// check if not outside of display
    if ((x >= ILI9488_WIDTH) || (y >= ILI9488_HEIGHT)) return;
    ILI9488_SetAddressWindow(y, x, y, x);
#endif

#ifdef LCD_ORIENTATION_PORTRAIT
	// check if not outside of display
    if ((y >= ILI9488_WIDTH) || (x >= ILI9488_HEIGHT)) return;
    ILI9488_SetAddressWindow(x, y, x, y);
#endif
    // write the actual pixel
    ILI9488_WriteData(color);

}

void ILI9488_DrawChar(uint16_t x, uint16_t y, uint8_t scale, char ch, uint16_t color, uint16_t bg_color)
{

#ifdef LCD_ORIENTATION_LANDSCAPE
	// swap X and Y in this orientation
	uint16_t temp = x;
	x = y;
	y = temp;
#endif

    // check if coordinates are not outside of display
    if ((y >= ILI9488_WIDTH) || (x >= ILI9488_HEIGHT)) return;

    // draw character, per pixel,
    // with selected character and background color

    uint8_t collumn = 0;

    //for (uint8_t i = 5 * scale; i > 0   ; i = i - scale)
    for (uint8_t i = 0; i < 5 * scale  ; i = i + scale)
    {
        for (uint8_t step_for_scaling2 = 0; step_for_scaling2 <  scale ; step_for_scaling2++)
        {
        	uint8_t line = font5x8[ch - 0x20][collumn];

			for (uint8_t j = 8 * scale; j > 0  ; j = j - scale)
			//for (uint8_t j = 0; j < 8 * scale ; j = j + scale)
			{
				 for (uint8_t step_for_scaling = 0; step_for_scaling <  scale ; step_for_scaling++)
				 {	/*
					if (line & 0x01)
						ILI9488_DrawPixel(x + j + step_for_scaling, y + i + step_for_scaling2 , color);
					else
						ILI9488_DrawPixel(x + j + step_for_scaling, y + i + step_for_scaling2, bg_color);
					*/
					if (line & 0x01)
						ILI9488_DrawPixel(y + i + step_for_scaling2, x + j + step_for_scaling , color);
					else
						ILI9488_DrawPixel(y + i + step_for_scaling2, x + j + step_for_scaling, bg_color);
				 }

				line >>= 1;
			}

        }

        collumn++;
    }

}


void ILI9488_DrawString(uint16_t x, uint16_t y, uint8_t scale, char *str, uint16_t color, uint16_t bgcolor)
{
    while (*str)
    {
    	ILI9488_DrawChar(x, y, scale, *str, color, bgcolor);
        x += 6 * scale; // character spacing
        str++;
    }
}


void ILI9488_DrawButton(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t scale, char *str, uint16_t color, uint16_t bgcolor)
{
	// plot button rectangle
	ILI9488_FilledRectangle(x1 ,y1 ,x2, y2, color); // rectangle at x, y, with, height, color
	// add button text
	ILI9488_DrawString(x1 + 10, y1 + 10, scale, str, color, bgcolor);

}

// draw a vertical line
// width = 1 pixel
void ILI9488_V_line(unsigned int x, unsigned int y, unsigned int length, unsigned int color)
{

#ifdef LCD_ORIENTATION_PORTRAIT
	// swap X and Y in this orientation
	ILI9488_SetAddressWindow(y, x, y+length, x);
#endif

#ifdef LCD_ORIENTATION_LANDSCAPE
	ILI9488_SetAddressWindow(x, y, x+length, y);
#endif

	// write actual pixel data
    for (uint32_t i = 0; i < length; i++)
    {
        ILI9488_WriteData(color);
    }

}

// draw a horizontal line
// width = 1 pixel
void ILI9488_H_line(unsigned int x, unsigned int y, unsigned int length, unsigned int color)
{

#ifdef LCD_ORIENTATION_PORTRAIT
	// swap X and Y in this orientation
	ILI9488_SetAddressWindow(y, x, y, x+length);
#endif

#ifdef LCD_ORIENTATION_LANDSCAPE
	ILI9488_SetAddressWindow(x, y, x, y+length);
#endif



    for (uint32_t i = 0; i < length; i++)
    {
        ILI9488_WriteData(color);
    }
}

// draws a rectangle from lines
// line thickness 1 pixel
// the rectangle is not filled
void ILI9488_Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int color)
{
#ifdef LCD_ORIENTATION_PORTRAIT
	ILI9488_H_line(x  , y  , w, color);
	ILI9488_H_line(x+h, y  , w, color);
	ILI9488_V_line(x  , y  , h, color);
	ILI9488_V_line(x  , y+w, h, color);
#endif

#ifdef LCD_ORIENTATION_LANDSCAPE
	ILI9488_H_line(y  , x  , w, color);
	ILI9488_H_line(y+h, x  , w, color);
	ILI9488_V_line(y  , x  , h, color);
	ILI9488_V_line(y  , x+w, h, color);
#endif

}

// draws a rectangle from lines
// line thickness 3 pixels
// the rectangle is not filled
void ILI9488_Rect_3(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int color)
{
#ifdef LCD_ORIENTATION_PORTRAIT
	// TODO: fix this, landscape is OK, portrait is wrong
	ILI9488_drawThickLine(x  , y  , x+w, y  , 3, color);
	ILI9488_drawThickLine(x+h, y  , x+h+w, y, 3, color);
	ILI9488_drawThickLine(x  , y  , x, y+h  , 3, color);
	ILI9488_drawThickLine(x  , y+w, x, y+w+h, 3, color);
#endif

#ifdef LCD_ORIENTATION_LANDSCAPE
	ILI9488_drawThickLine(y  , x  , y+w, x, 3, color);
	ILI9488_drawThickLine(y  , x+h, y+w, x+h, 3, color);
	ILI9488_drawThickLine(y  , x  , y, x+h, 3, color);
	ILI9488_drawThickLine(y+w, x  , y+w, x+h,3, color);
#endif

}

// draw a thick line from point x0,y0
// to point x1,y1
// Use Bresenham’s line algorithm
// Compute points along the line from (x1, y1) to (x2, y2)
// Around each point, draw a small rectangle (or square) with side equal to thickness

void ILI9488_drawThickLine(int x0, int y0, int x1, int y1, uint8_t thickness, uint16_t color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    int e2;

    int half_thick = thickness / 2;

    while (1)
    {
        // Draw a filled rectangle centered at (x0, y0) with size thickness x thickness
    	ILI9488_FilledRectangle(x0 - half_thick, y0 - half_thick, x0 + half_thick, y0 + half_thick, color);

        if (x0 == x1 && y0 == y1) break;

        e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}



// Plot a filled ring by setting pixels between two radii
// Loops over a square region covering the circle.
// For each pixel (x, y) relative to the center (x0, y0), calculates x² + y².
// If the pixel is within the desired ring (between inner² and outer²), it gets drawn.

void ILI9488_drawCircleOutline(int x0, int y0, int radius, uint8_t thickness, uint16_t color)
{
    if (radius <= 0 || thickness == 0) return;

    int outer = radius;
    int inner = radius - thickness;
    if (inner < 0) inner = 0;

    for (int y = -outer; y <= outer; y++)
    {
        for (int x = -outer; x <= outer; x++)
        {
            int r2 = x * x + y * y;

            if (r2 >= inner * inner && r2 <= outer * outer)
            {
                ILI9488_DrawPixel(x0 + x, y0 + y, color);
            }
        }
    }
}

void ILI9488_DrawBatterySymbol(uint16_t x, uint16_t y, uint16_t size, uint16_t color)
{
    // Battery aspect ratio
    uint16_t width = size / 2;            // battery width
    uint16_t height = size;               // battery height

    // Battery body rectangle (excluding terminal)
    uint16_t body_x0 = x;
    uint16_t body_y0 = y;
    uint16_t body_x1 = width;
    uint16_t body_y1 = height;

    // Draw battery body outline
	#ifdef LCD_ORIENTATION_LANDSCAPE
		ILI9488_Rect_3(y, x, width, height, color);
	#endif
	#ifdef LCD_ORIENTATION_PORTRAIT
		// TODO: untested
		ILI9488_Rect_3(x, y, width, height, color);
	#endif

    // Terminal (positive pole)
    uint16_t terminal_height = size / 12;
    uint16_t terminal_width  = width / 3;
    uint16_t terminal_x = x + width / 2 - terminal_width / 2;
    uint16_t terminal_y = y + height;

    // Draw terminal
    //ILI9488_DrawPixel(terminal_x, terminal_y, WHITE);
    ILI9488_FilledRectangle(terminal_x, terminal_y, terminal_x + terminal_width, terminal_y + terminal_height, color);



    // Internal bar padding
    uint16_t bar_count = 4;
    uint16_t bar_gap = 4;
    uint16_t bar_height = (height - ((bar_count + 1) * bar_gap)) / bar_count;
    uint16_t bar_width = width - 5; // a bit narrower to fit inside

    // Draw filled bars — 3 out of 4 = 75%
    for (uint8_t i = 0; i < 3; i++)
    {
        uint16_t bx = x + 4; // left inset
        uint16_t by = body_y0 + bar_gap + i * (bar_height + bar_gap); // from bottom up
        ILI9488_FilledRectangle(bx, by, bx + bar_width, by + bar_height, color);
      //  uint16_t by = body_y1 - bar_gap - (i + 1) * (bar_height + bar_gap); // from bottom up
      //  ILI9488_FilledRectangle(bx, by, bx + bar_width, by + bar_height, color);
    }
}
