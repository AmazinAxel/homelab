#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include "lib.h"
#include <stdbool.h> // Boolean

extern bool isLCDOn;

typedef enum { OVERVIEW, LOGS, SHUTDOWN } MenuType;
extern MenuType currentMenu;

void initLCD();
void buttonHandler();
void menu(MenuType menu);

bool isSynced();
bool isNetworkConnected();
char* storageUsage();

#endif
