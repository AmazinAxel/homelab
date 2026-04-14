
#ifndef LCD_HEIGHT

#include "lib.h"
#include <stdint.h>

#include <stdio.h>

#define LCD_HEIGHT 128
#define LCD_WIDTH 128

#define LCD_X 2
#define LCD_X_MAXPIXEL 132
#define LCD_Y 1
#define LCD_Y_MAXPIXEL 162
#define HORIZONTAL 0
#define VERTICAL 1
#define LCD_WIDTH_Byte 240

typedef enum{
	L2R_U2D  = 0,
	L2R_D2U  ,
	R2L_U2D  ,
	R2L_D2U  ,

	U2D_L2R  ,
	U2D_R2L  ,
	D2U_L2R  ,
	D2U_R2L  , 
} LCD_SCAN_DIR;
#define SCAN_DIR_DFT U2D_R2L

typedef struct{
	UWORD LCD_Dis_Column;
	UWORD LCD_Dis_Page;
	LCD_SCAN_DIR LCD_Scan_Dir;
	UWORD LCD_X_Adjust;
	UWORD LCD_Y_Adjust;
} LCD_DIS;


void turnOffLCD();
void turnOnLCD();
void LCD_1in44_Init(LCD_SCAN_DIR Scan_dir);
void LCD_1in44_Display();
void LCD_1in44_DisplayWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD *Image);
void Handler_1in44_LCD(int signo);
void LCD_1in44_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend);

#endif
