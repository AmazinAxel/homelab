#include "../LCD/LCD_1in44.h"
#include "../GUI/GUI_Paint.h"
#include <stdio.h> // #printf
#include <stdlib.h>	// #exit
#include <signal.h> // #signal

void overviewMenu()
{
    // control + C handler
    signal(SIGINT, Handler_1in44_LCD);

	if(DEV_ModuleInit() != 0){
        DEV_ModuleExit();
        exit(0);
    };

	LCD_SCAN_DIR LCD_ScanDir = SCAN_DIR_DFT;

    LCD_1in44_Init(LCD_ScanDir);
	LCD_1in44_Clear(WHITE);

    UWORD *BlackImage;
    UDOUBLE Imagesize = LCD_HEIGHT*LCD_WIDTH*2;
    if((BlackImage = (UWORD *)malloc(Imagesize)) == NULL) {
        printf("Can't allocate black memory\r\n");
        exit(0);
    }

    // Image cache
    Paint_NewImage(BlackImage, LCD_WIDTH, LCD_HEIGHT, 0, WHITE, 16);
    Paint_Clear(WHITE);

    Paint_DrawLine(2, 10, sLCD_DIS.LCD_Dis_Column - 1 , 10, BLUE,DOT_PIXEL_8X8, LINE_STYLE_SOLID);

	Paint_DrawString_EN(0, 0, "Homelab", &Font24, BLUE4, BLACK);
	Paint_DrawString_EN(32, 33, "Backed up:", &Font12, WHITE, BLACK);
	Paint_DrawString_EN(28, 45, "Network:", &Font8, WHITE, BLACK);
	Paint_DrawString_EN(28, 45, "Network:", &Font8, WHITE, BLACK);

    LCD_1in44_Display(BlackImage);
    DEV_Delay_ms(2000);        
    LCD_1in44_Display(BlackImage);
    DEV_Delay_ms(2000);
    
    /* Module Exit */
    free(BlackImage);
    BlackImage = NULL;
	DEV_ModuleExit();
}

