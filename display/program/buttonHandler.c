#include "program.h"
#include "GUI_Paint.h"
#include "LCD_1in44.h"
#include "KEY_APP.h"
#include <stdlib.h> 
#include <stdio.h>

void ButtonHandler(UWORD *Image);

void InitButtonHandler(void) {
    UWORD *BlackImage;
    UWORD Imagesize = LCD_HEIGHT*LCD_WIDTH;
    
    Paint_NewImage(BlackImage, LCD_WIDTH, LCD_HEIGHT, 0, WHITE, 16);
    Paint_Clear(WHITE);
   
    ButtonHandler(BlackImage);
    
    /* Module Exit */
    free(BlackImage);
    BlackImage = NULL;
	DEV_ModuleExit();
}



void ButtonHandler(UWORD *Image)
{
    for(;;) {
        if(GET_KEY_UP == 0) {

        }
        if(GET_KEY_DOWN == 0) {
            while(GET_KEY_DOWN == 0) {
                Paint_DrawRectangle(40, 80, 60, 100, BLUE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
                GUI_Partial_Refresh(40, 80, 60, 100);
            }
            Paint_DrawRectangle(40, 80, 60, 100, WHITE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
            Paint_DrawRectangle(40, 80, 60, 100, RED, DOT_PIXEL_DFT, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(43, 80, "D", &Font24, IMAGE_BACKGROUND, BLUE);
            GUI_Partial_Refresh(40, 80, 60, 100);
        }
        if(GET_KEY_LEFT == 0) {
            while(GET_KEY_LEFT == 0) {
                Paint_DrawRectangle(20, 60, 40, 80, BLUE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
                GUI_Partial_Refresh(20, 60, 40, 80);
            }
            Paint_DrawRectangle(20, 60, 40, 80, WHITE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
            Paint_DrawRectangle(20, 60, 40, 80, RED, DOT_PIXEL_DFT, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(23, 60, "L", &Font24, IMAGE_BACKGROUND, BLUE);
            GUI_Partial_Refresh(20, 60, 40, 80);
        }
        if(GET_KEY_RIGHT == 0) {
            while(GET_KEY_RIGHT == 0) {
                Paint_DrawRectangle(60, 60, 80, 80, BLUE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
                GUI_Partial_Refresh(60, 60, 80, 80);
            }
            Paint_DrawRectangle(60, 60, 80, 80, WHITE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
            Paint_DrawRectangle(60, 60, 80, 80, RED, DOT_PIXEL_DFT, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(63, 60, "R", &Font24, IMAGE_BACKGROUND, BLUE);
            GUI_Partial_Refresh(60, 60, 80, 80);
        }
        if(GET_KEY_PRESS == 0) {
            while(GET_KEY_PRESS == 0) {
                Paint_DrawRectangle(40, 60, 60, 80, BLUE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
                GUI_Partial_Refresh(40, 60, 60, 80);
            }
            Paint_DrawRectangle(40, 60, 60, 80, WHITE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
            Paint_DrawRectangle(40, 60, 60, 80, RED, DOT_PIXEL_DFT, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(43, 60, "P", &Font24, IMAGE_BACKGROUND, BLUE);
            GUI_Partial_Refresh(40, 60, 60, 80);
        }
        if(GET_KEY1 == 0) {
            while(GET_KEY1 == 0) {
                Paint_DrawRectangle(95, 40, 120, 60, BLUE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
                GUI_Partial_Refresh(95, 40, 120, 60);
            }
            Paint_DrawRectangle(95, 40, 120, 60, WHITE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
            Paint_DrawRectangle(95, 40, 120, 60, RED, DOT_PIXEL_DFT, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(98, 43, "K1", &Font16, IMAGE_BACKGROUND, BLUE);
            GUI_Partial_Refresh(95, 40, 120, 60);
        }
        if(GET_KEY2 == 0) {
            while(GET_KEY2 == 0) {
                Paint_DrawRectangle(95, 60, 120, 80, BLUE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
                GUI_Partial_Refresh(95, 60, 120, 80);
            }
            Paint_DrawRectangle(95, 60, 120, 80, WHITE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
            Paint_DrawRectangle(95, 60, 120, 80, RED, DOT_PIXEL_DFT, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(98, 63, "K2", &Font16, IMAGE_BACKGROUND, BLUE);
            GUI_Partial_Refresh(95, 60, 120, 80);
        }
        if(GET_KEY3 == 0) {
            while(GET_KEY3 == 0) {
                Paint_DrawRectangle(95, 80, 120, 100, BLUE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
                GUI_Partial_Refresh(95, 80, 120, 100);
            }
            Paint_DrawRectangle(95, 80, 120, 100, WHITE, DOT_PIXEL_DFT, DRAW_FILL_FULL);
            Paint_DrawRectangle(95, 80, 120, 100, RED, DOT_PIXEL_DFT, DRAW_FILL_EMPTY);
            Paint_DrawString_EN(98, 83, "K3", &Font16, IMAGE_BACKGROUND, BLUE);
            GUI_Partial_Refresh(95, 80, 120, 100);
        }
    }
}