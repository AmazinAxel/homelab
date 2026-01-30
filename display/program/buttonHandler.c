#include "program.h"
#include "LCD_1in44.h" // LCD power control
#include <time.h> // Button hold conditions
#include <unistd.h> // Sleeping
#include <stdbool.h> // Booleans

static int buttonHold(int (*read_fn)(void)) {
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (read_fn() == 0) {
        clock_gettime(CLOCK_MONOTONIC, &now);

        long ms = ((now.tv_sec - start.tv_sec) * 1000) + ((now.tv_nsec - start.tv_nsec) / 1000000);

        if (ms >= 500) { return true; }; // 500ms

        usleep(1000); // 1ms
    };
    return false;
};

inline int GET_KEY_PRESS(void) { return DEV_Digital_Read(KEY_PRESS_PIN); }
inline int GET_KEY1(void) { return DEV_Digital_Read(KEY1_PIN); }
inline int GET_KEY2(void) { return DEV_Digital_Read(KEY2_PIN); }
inline int GET_KEY3(void) { return DEV_Digital_Read(KEY3_PIN); }


void buttonHandler() {
    for (;;) {
        //if (GET_KEY_UP == 0) {
        //    while (GET_KEY_UP == 0);
        //};

        //if (GET_KEY_DOWN == 0) {
        //    while (GET_KEY_DOWN == 0);
        //};

        //if (GET_KEY_LEFT == 0) {
        //    while (GET_KEY_LEFT == 0);
        //};

        //if (GET_KEY_RIGHT == 0) {
        //    while (GET_KEY_RIGHT == 0);
        //};

        if (GET_KEY_PRESS() == 0) { // Toggle screen sleep
            if (isLCDOn) {
                turnOffLCD();
                isLCDOn = false;
            } else {
                turnOnLCD();
                isLCDOn = true;
            };

            while (GET_KEY1() == 0);
        };

        if (GET_KEY1() == 0) { // Overview
            if (currentMenu != OVERVIEW) {
                menu(OVERVIEW);
            };

            if (buttonHold(GET_KEY1)) {
                // held down
            };
            while (GET_KEY1() == 0);
        };

        if (GET_KEY2() == 0) { // Logs
            if (currentMenu != LOGS) {
                menu(LOGS);
            };

            if (buttonHold(GET_KEY2)) {
                // held down
            };
            while (GET_KEY2() == 0);
        };

        if (GET_KEY3() == 0) { // Shutdown
            if (currentMenu != SHUTDOWN) {
                menu(SHUTDOWN);
            };
            
            if (buttonHold(GET_KEY3)) {
                if (system("systemctl poweroff") == -1) { };
            };
            while (GET_KEY3() == 0);
        };
    };
};
