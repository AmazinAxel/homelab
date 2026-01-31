#include "lcd.h"
#include "draw.h"
#include "menus/menus.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

UWORD *screen = NULL;
bool isLCDOn = true;
MenuType currentMenu = OVERVIEW;

void initLCD() {
    signal(SIGINT, Handler_1in44_LCD);

    if (DEV_ModuleInit() != 0) {
        DEV_ModuleExit();
        exit(1);
    };

    LCD_SCAN_DIR LCD_ScanDir = SCAN_DIR_DFT;
    LCD_1in44_Init(LCD_ScanDir);

    UDOUBLE imageSize = LCD_HEIGHT * LCD_WIDTH * 2;
    screen = malloc(imageSize);

    if (!screen) {
        printf("Can't allocate black memory\n");
        exit(1);
    };

    Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, WHITE, 16);
};

void menu(MenuType menu) {
    Paint_ClearWindow(0, 0, 128, 128, BACKGROUND_COLOR);

    currentMenu = menu;

    if (!isLCDOn)
        turnOnLCD();

    switch (menu) {
        case OVERVIEW:
            overviewMenu();
            break;
        case LOGS:
            logsMenu();
            break;
        case SHUTDOWN:
            shutdownMenu();
            break;
    };
    LCD_1in44_Display();
};
