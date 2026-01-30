#include "../LCD/LCD_1in44.h"
#include "../GUI/GUI_Paint.h"

void logsMenu() {
    Paint_DrawString(4, 4, "Logs", &Font20, BACKGROUND_COLOR, BLUE3);

    FILE *logFile;
    logFile = fopen("/home/alec/logs.txt", "r");
    char line[100]; // Max line size
    int lineY = 20;

    while (fgets(line, sizeof line, logFile)) {
        printf("Log entry: %s\n", line);
        Paint_DrawString(4, lineY, line, &Font12, BACKGROUND_COLOR, FONT_COLOR);

        lineY += 12;
    };

    fclose(logFile);
};
