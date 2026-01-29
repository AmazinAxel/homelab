// Top title reading "Logs:"
// Read files from /home/alec/logs.txt and show here

#include "../LCD/LCD_1in44.h"
#include "../GUI/GUI_Paint.h"

void logsMenu(UWORD *BlackImage) {
    Paint_NewImage(BlackImage, LCD_WIDTH, LCD_HEIGHT, BACKGROUND_COLOR, 16);
    Paint_ClearWindow(0, 0, 128, 128, BACKGROUND_COLOR);

    Paint_DrawString_EN(3, 3, "Logs", &Font24, BLACK4, BLUE3);
};
