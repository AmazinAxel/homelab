#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include "DEV_Config.h" // for UWORD
#include <stdbool.h> // Boolean

extern bool isLCDOn;

typedef enum {
    OVERVIEW,
    LOGS,
    SHUTDOWN
} MenuType;

void initLCD();
void buttonHandler();
void menu(MenuType menu);

#endif
