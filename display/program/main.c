#include "program.h"
#include "lcd.h"

int main() {
    initLCD();
    menu(OVERVIEW);
    turnOffLCD();
    isLCDOn = false;
    buttonHandler();
    DEV_ModuleExit();
	return 0;
};
