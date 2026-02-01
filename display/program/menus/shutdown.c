#include "lcd.h"
#include "draw.h"

void shutdownMenu() {
    Paint_DrawString(14, 43, "Hold to", &Font20, BACKGROUND_COLOR, BLUE3);
    Paint_DrawString(8, 68, "shutdown", &Font20, BACKGROUND_COLOR, BLUE3);
};
