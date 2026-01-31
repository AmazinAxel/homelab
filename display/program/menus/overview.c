#include "lcd.h"
#include "draw.h"
#include "program.h"

void overviewMenu() {
    Paint_DrawLine(8, 8, 128 - 7, 8, BLACK4, DOT_PIXEL_8X8);
    Paint_DrawLine(8, 17 + 4, 128 - 7, 17 + 4, BLACK4, DOT_PIXEL_8X8);
    Paint_DrawString(5, 3, "homelab", &Font24, BLACK4, BLUE3);

    // Is backed up
    Paint_DrawString(8, 45, "Backed up:", &Font12, BACKGROUND_COLOR, FONT_COLOR);
    if (isSynced()) {
        Paint_DrawCheck(50, 45);
    } else {
        Paint_DrawX(50, 45);
    };

    // Is connected to network
    Paint_DrawString(8, 70, "Network:", &Font12, BACKGROUND_COLOR, FONT_COLOR);

    //if (isNetworkConnected) {
        Paint_DrawCheck(50, 70);
    //} else {
        Paint_DrawX(50, 85);
    //};

    // Drive storage usage
    Paint_DrawString(8, 95, "Storage:", &Font12, BACKGROUND_COLOR, FONT_COLOR);
    Paint_DrawString(8, 95, storageUsage(), &Font12, BACKGROUND_COLOR, BLUE);

};
