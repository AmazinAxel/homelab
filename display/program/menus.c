#include "LCD_1in44.h"
#include "GUI_Paint.h"
#include "menus/menus.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void menu(MenuType menu) {
    signal(SIGINT, Handler_1in44_LCD);

    if (DEV_ModuleInit() != 0) {
        DEV_ModuleExit();
        exit(0);
    };

    LCD_SCAN_DIR LCD_ScanDir = SCAN_DIR_DFT;
    LCD_1in44_Init(LCD_ScanDir);
    LCD_1in44_Clear(WHITE);

    UDOUBLE Imagesize = LCD_HEIGHT * LCD_WIDTH * 2;
    UWORD *BlackImage = malloc(Imagesize);
    if (!BlackImage) {
        printf("Can't allocate black memory\r\n");
        exit(0);
    };

     switch (menu) {
        case OVERVIEW:
            overviewMenu(BlackImage);
            break;
        case LOGS:
            logsMenu(BlackImage);
            break;
        case SHUTDOWN:
            shutdownMenu(BlackImage);
            break;
     };

    LCD_1in44_Display(BlackImage);
    DEV_Delay_ms(2000);

    free(BlackImage);
    //DEV_ModuleExit();
}
