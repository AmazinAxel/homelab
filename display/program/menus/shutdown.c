#include "../LCD/LCD_1in44.h"
#include "../GUI/GUI_Paint.h"

void shutdownMenu() {
    Paint_DrawString(13, 42, "Hold to", &Font20, BACKGROUND_COLOR, BLUE3);
    Paint_DrawString(8, 67, "shutdown", &Font20, BACKGROUND_COLOR, BLUE3);
};
