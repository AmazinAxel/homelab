// Hold to shutdown text

// Top title reading "Logs:"
// Read files from /home/alec/logs.txt and show here

#include "../LCD/LCD_1in44.h"
#include "../GUI/GUI_Paint.h"

void shutdownMenu() {
    Paint_DrawString_EN(7, 20, "Hold to", &Font16, BLACK4, BLUE3);
    Paint_DrawString_EN(11, 60, "Shutdown", &Font16, BLACK4, BLUE3);
};
