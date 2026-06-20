#include "program.h"
#include "lcd.h" // LCD power control
#include <time.h> // Button hold timing
#include <unistd.h> // usleep(), truncate()
#include <stdbool.h> // Booleans
#include <stdlib.h> // system()

#define DEBOUNCE_US 15000
#define HOLD_MS 500

static bool buttonHold(UWORD pin) {
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (DEV_Button_Read(pin) == 0) {
        clock_gettime(CLOCK_MONOTONIC, &now);

        long ms = ((now.tv_sec - start.tv_sec) * 1000) + ((now.tv_nsec - start.tv_nsec) / 1000000);
        if (ms >= HOLD_MS) { return true; };

        usleep(5000); // 5ms
    };
    return false;
};

static void waitForRelease(UWORD pin) {
    while (DEV_Button_Read(pin) == 0) {
        usleep(10000); // 10ms
    };
};

static void handlePress(UWORD pin) {
    if (pin == KEY_PRESS_PIN) { // Toggle screen sleep
        if (isLCDOn) {
            turnOffLCD();
            isLCDOn = false;
        } else {
            turnOnLCD();
            isLCDOn = true;
        };
    } else if (pin == KEY1_PIN) { // Overview
        if (currentMenu != OVERVIEW || buttonHold(KEY1_PIN)) {
            menu(OVERVIEW);
        };
    } else if (pin == KEY2_PIN) { // Logs
        if (currentMenu != LOGS) {
            menu(LOGS);
        };

        if (buttonHold(KEY2_PIN)) { // Hold to wipe logs
            if (truncate("/home/alec/logs.txt", 0) == -1) { };
            menu(LOGS);
        };
    } else if (pin == KEY3_PIN) { // Shutdown
        if (currentMenu != SHUTDOWN) {
            menu(SHUTDOWN);
        };

        if (buttonHold(KEY3_PIN)) { // Hold to power off
            if (system("systemctl poweroff") == -1) { };
        };
    };
};

void buttonHandler() {
    DEV_Button_Init();

    for (;;) {
        int pin = DEV_Button_WaitPress(-1);
        if (pin < 0) { continue; }; // timeout/signal/release edge

        usleep(DEBOUNCE_US);
        if (DEV_Button_Read((UWORD)pin) != 0) { continue; }; // bounce

        handlePress((UWORD)pin);
        waitForRelease((UWORD)pin);
    };
};
