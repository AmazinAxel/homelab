#include "lcd.h"
#include "draw.h"

void shutdownMenu() {
    Paint_DrawString(13, 42, "Hold to", &Font20, BACKGROUND_COLOR, BLUE3);
    Paint_DrawString(8, 67, "shutdown", &Font20, BACKGROUND_COLOR, BLUE3);
};
