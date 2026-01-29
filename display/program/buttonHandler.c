#include "program.h"

void ButtonHandler() {
    for (;;) {
        if (GET_KEY_UP == 0) {
            while (GET_KEY_DOWN == 0) {
                return;
            };
        
        } else if (GET_KEY_DOWN == 0) {
            while (GET_KEY_DOWN == 0) {
                return;
            };

        } else if (GET_KEY_LEFT == 0) {
            while (GET_KEY_LEFT == 0) {
                return;
            };
        
        } else if (GET_KEY_RIGHT == 0) {
            while (GET_KEY_RIGHT == 0) {
                return;
            };
        }

        else if (GET_KEY_PRESS == 0) {
            while (GET_KEY_PRESS == 0) {
                return;
            };
        
        } else if (GET_KEY1 == 0) {
            while (GET_KEY1 == 0) {
                return;
            };
        
        } else if(GET_KEY2 == 0) {
            while(GET_KEY2 == 0) {
                return;
            };

        } else if (GET_KEY3 == 0) {
            while (GET_KEY3 == 0) {
                return;
            };
        };
    };
};