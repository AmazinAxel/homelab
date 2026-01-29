#include "../LCD/LCD_1in44.h"
#include "../GUI/GUI_Paint.h"

void overviewMenu(UWORD *BlackImage) {
    Paint_NewImage(BlackImage, LCD_WIDTH, LCD_HEIGHT, BACKGROUND_COLOR, 16);
    Paint_ClearWindow(0, 0, 128, 128, BACKGROUND_COLOR);

    Paint_DrawLine(8, 8, 128 - 7, 8, BLACK4, DOT_PIXEL_8X8, LINE_STYLE_SOLID);
    Paint_DrawLine(8, 17 + 4, 128 - 7, 17 + 4, BLACK4, DOT_PIXEL_8X8, LINE_STYLE_SOLID);
    Paint_DrawString_EN(6, 3, "homelab", &Font24, BLACK4, BLUE3);

    Paint_DrawString_EN(8, 45, "Backed up:", &Font12, BACKGROUND_COLOR, FONT_COLOR);
    Paint_DrawString_EN(8, 70, "Network:", &Font12, BACKGROUND_COLOR, FONT_COLOR);
    Paint_DrawString_EN(8, 95, "Storage:", &Font12, BACKGROUND_COLOR, FONT_COLOR);
};
