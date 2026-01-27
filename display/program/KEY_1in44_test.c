#include "DEV_Config.h"
#include "LCD_1in44.h"
#include "GUI_Paint.h"
#include "KEY_APP.h"

#include "test.h"

#include <stdio.h>		//printf()
#include <stdlib.h>		//exit()
#include <signal.h>     //signal()
#include <time.h>  

void KEY_1in44_test(void) {
    UWORD *BlackImage;
    UWORD Imagesize = LCD_HEIGHT*LCD_WIDTH;
    
    Paint_NewImage(BlackImage, LCD_WIDTH, LCD_HEIGHT, 0, WHITE, 16);
    Paint_Clear(WHITE);
   
    /* Monitor button */
    printf("Listening KEY\r\n");
    KEY_Listen(BlackImage);
    
    /* Module Exit */
    free(BlackImage);
    BlackImage = NULL;
	DEV_ModuleExit();
}

