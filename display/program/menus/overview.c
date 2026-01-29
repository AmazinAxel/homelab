#include "../LCD/LCD_1in44.h"
#include "../GUI/GUI_Paint.h"
#include <stdio.h> // #printf
#include <stdlib.h>	// #exit
#include <signal.h> // #signal

extern LCD_DIS sLCD_DIS;

void overviewMenu(UWORD *BlackImage) {
    DEV_Delay_ms(100); // color fix?

    Paint_NewImage(BlackImage, LCD_WIDTH, LCD_HEIGHT, BACKGROUND_COLOR, 16);
    //Paint_Clear(0x0000);
    Paint_ClearWindow(0, 0, 128, 128, BACKGROUND_COLOR);

    Paint_DrawLine(8, 8, 128 - 7, 8, BLACK, DOT_PIXEL_8X8, LINE_STYLE_SOLID);

    Paint_DrawLine(8, 17 + 4, 128 - 7, 17 + 4, BLACK, DOT_PIXEL_8X8, LINE_STYLE_SOLID);

    Paint_DrawString_EN(6, 3, "Homelab", &Font24, BLACK, BLUE2);
    Paint_DrawString_EN(0, 40, "Backed up:", &Font20, BLACK4, WHITE);
    Paint_DrawString_EN(0, 65, "Network:", &Font20, BLACK4, WHITE);
    Paint_DrawString_EN(0, 90, "Storage:", &Font20, BLACK4, WHITE);
};
