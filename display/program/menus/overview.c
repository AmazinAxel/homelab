#include "lcd.h"
#include "draw.h"

void overviewMenu() {
    Paint_DrawLine(8, 8, 128 - 7, 8, BLACK4, DOT_PIXEL_8X8);
    Paint_DrawLine(8, 17 + 4, 128 - 7, 17 + 4, BLACK4, DOT_PIXEL_8X8);
    Paint_DrawString(5, 3, "homelab", &Font24, BLACK4, BLUE3);

    // Is backed up
    Paint_DrawString(8, 45, "Backed up:", &Font12, BACKGROUND_COLOR, FONT_COLOR);

    // Is connected to network
    Paint_DrawString(8, 70, "Network:", &Font12, BACKGROUND_COLOR, FONT_COLOR);

    // Drive storage usage
    Paint_DrawString(8, 95, "Storage:", &Font12, BACKGROUND_COLOR, FONT_COLOR);
};
