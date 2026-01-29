#include "program.h"
#include "LCD_1in44.h" // LCD power control

static int IsLCDOn = 1;

void buttonHandler() {
    for (;;) {
        if (GET_KEY_UP == 0) {
            while (GET_KEY_UP == 0);
            //continue;
        };

        if (GET_KEY_DOWN == 0) {
            while (GET_KEY_DOWN == 0);
            //continue;
        };

        if (GET_KEY_LEFT == 0) {
            while (GET_KEY_LEFT == 0);
            //continue;
        };

        if (GET_KEY_RIGHT == 0) {
            while (GET_KEY_RIGHT == 0);
            //continue;
        };

        if (GET_KEY_PRESS == 0) {
            if (IsLCDOn) {
                turnOffLCD();
                IsLCDOn = 0;
            } else {
                turnOnLCD();
                IsLCDOn = 1;
            };

            while (GET_KEY_PRESS == 0);
            //continue;
        };

        if (GET_KEY1 == 0) {
            menu(OVERVIEW);
            while (GET_KEY1 == 0);
            //continue;
        };

        if (GET_KEY2 == 0) {
            menu(LOGS);
            while (GET_KEY2 == 0);
            //continue;
        };

        if (GET_KEY3 == 0) {
            menu(SHUTDOWN);
            while (GET_KEY3 == 0);
            //continue;
        };
    };
};
