#include "program.h"

int main() {
    initLCD();
    menu(OVERVIEW);
    buttonHandler();
    DEV_ModuleExit();
	return 0;
};
