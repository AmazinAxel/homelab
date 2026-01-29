#include "../LCD/LCD_1in44.h"
#include "../GUI/GUI_Paint.h"
#include <stdio.h> // #printf
#include <stdlib.h>	// #exit
#include <signal.h> // #signal

extern LCD_DIS sLCD_DIS;

void overviewMenu(UWORD *BlackImage) {
    Paint_NewImage(BlackImage, LCD_WIDTH, LCD_HEIGHT, 0, WHITE, 16);
    Paint_Clear(WHITE);

    Paint_DrawLine(2, 10, sLCD_DIS.LCD_Dis_Column - 1, 10, BLUE, DOT_PIXEL_8X8, LINE_STYLE_SOLID);

    Paint_DrawString_EN(0, 0, "Homelab", &Font24, BLUE4, BLACK);
    Paint_DrawString_EN(32, 0, "Backed up:", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(45, 0, "Network:", &Font8, WHITE, BLACK);
    Paint_DrawString_EN(60, 0, "Storage:", &Font8, WHITE, BLACK);
}
