// Top title reading "Logs:"
// Read files from /home/alec/logs.txt and show here

#include "../LCD/LCD_1in44.h"
#include "../GUI/GUI_Paint.h"

void logsMenu() {

    Paint_DrawString_EN(3, 3, "Logs", &Font24, BLACK4, BLUE3);
};
