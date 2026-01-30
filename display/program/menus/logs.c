#include "../LCD/LCD_1in44.h"
#include "../GUI/GUI_Paint.h"
#include <string.h>

void logsMenu() {
    Paint_DrawString(4, 4, "Logs", &Font20, BACKGROUND_COLOR, BLUE3);

    FILE *logFile;
    logFile = fopen("/home/alec/logs.txt", "r");
    char line[100]; // Max line size
    int lineY = 32;

    while (fgets(line, sizeof line, logFile)) {

        // Get rid of newlines or extra spaces because that segfaults the drawString function
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        Paint_DrawString(4, lineY, line, &Font12, BACKGROUND_COLOR, FONT_COLOR);

        lineY += 15;
    };

    fclose(logFile);
};