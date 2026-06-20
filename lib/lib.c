#include "lib.h"
#include <gpiod.h> // gpio
#include <fcntl.h> // spi open
#include <poll.h>  // blocking wait on button events

#define GPIO_CHIP "/dev/gpiochip0"

static struct gpiod_chip *chip;

static int spi_fd = -1;

static const UWORD button_pins[] = { KEY_PRESS_PIN, KEY1_PIN, KEY2_PIN, KEY3_PIN }; // <----
#define BUTTON_COUNT ((int)(sizeof(button_pins) / sizeof(button_pins[0])))

static struct gpiod_line *button_lines[BUTTON_COUNT];
static struct pollfd button_fds[BUTTON_COUNT];

void DEV_Digital_Write(UWORD Pin, UBYTE Value) {
    if (Pin == LCD_CS) {
        return;
    };

    if (!chip) {
        printf("DEV_Digital_Write: chip not initialized\n");
        return;
    };

    struct gpiod_line *line = gpiod_chip_get_line(chip, Pin);
    if (!line) {
        printf("DEV_Digital_Write: failed to get line %u\n", Pin);
        return;
    };

    if (gpiod_line_set_value(line, Value) < 0) {
        printf("DEV_Digital_Write: failed to set value on line %u\n", Pin);
    };
};

void DEV_Button_Init() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        button_fds[i].fd = -1; // poll() ignores negative fds
        button_fds[i].events = POLLIN;

        struct gpiod_line *line = gpiod_chip_get_line(chip, button_pins[i]);
        if (!line) {
            printf("DEV_Button_Init: failed to get line %u\n", button_pins[i]);
            continue;
        };

        if (gpiod_line_request_both_edges_events(line, "homelabDisplay") < 0) {
            printf("DEV_Button_Init: failed to request events on line %u\n", button_pins[i]);
            continue;
        };

        button_lines[i] = line;
        button_fds[i].fd = gpiod_line_event_get_fd(line);
    };
};

int DEV_Button_WaitPress(int timeout_ms) {
    int ready = poll(button_fds, BUTTON_COUNT, timeout_ms);
    if (ready <= 0) {
        return -1; // timeout or interrupted by a signal
    };

    int pressed = -1;
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (!button_lines[i] || !(button_fds[i].revents & POLLIN)) {
            continue;
        };

        struct gpiod_line_event event;
        if (gpiod_line_event_read(button_lines[i], &event) < 0) {
            continue;
        };

        if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE && pressed < 0) {
            pressed = (int)button_pins[i];
        };
    };

    return pressed;
};

int DEV_Button_Read(UWORD Pin) {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (button_pins[i] == Pin && button_lines[i]) {
            int v = gpiod_line_get_value(button_lines[i]);
            return (v < 0) ? 1 : v;
        };
    };
    return 1;
};

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode) {
    if (Pin == LCD_CS) {
        return;
    };

    if (!chip) {
        printf("DEV_GPIO_Mode: chip not initialized\n");
        return;
    };

    struct gpiod_line *line = gpiod_chip_get_line(chip, Pin);
    if (!line) {
        printf("DEV_GPIO_Mode: failed to get line %u\n", Pin);
        return;
    };

    if (Mode == 0) {
        if (gpiod_line_request_input(line, "waveshare") < 0) {
            printf("DEV_GPIO_Mode: failed to request input on line %u\n", Pin);
        };
    } else {
        if (gpiod_line_request_output(line, "waveshare", 0) < 0) {
            printf("DEV_GPIO_Mode: failed to request output on line %u\n", Pin);
        };
    };
};

void DEV_Delay_ms(UDOUBLE xms) {
    usleep(xms * 1000);
};

void DEV_SPI_WriteByte(uint8_t Value) {
    if (spi_fd < 0) {
        printf("DEV_SPI_WriteByte: SPI not initialized\n");
        return;
    };

    ssize_t written = write(spi_fd, &Value, 1);
    if (written != 1) {
        printf("DEV_SPI_WriteByte: write failed (ret=%zd)\n", written);
    };
};

void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len) {
    if (spi_fd < 0) {
        printf("DEV_SPI_Write_nByte: SPI not initialized\n");
        return;
    };

    ssize_t written = write(spi_fd, pData, Len);
    if (written < 0) {
        printf("DEV_SPI_Write_nByte: write failed\n");
    } else if ((uint32_t)written != Len) {
        printf("DEV_SPI_Write_nByte: partial write (%zd/%u)\n", written, Len);
    };
};

static void DEV_GPIO_Init() {
    DEV_GPIO_Mode(LCD_RST, 1);
    DEV_GPIO_Mode(LCD_DC, 1);
    DEV_GPIO_Mode(LCD_BL, 1);

    DEV_Digital_Write(LCD_BL, 1); // Backlight on
};

UBYTE DEV_ModuleInit() {
    // GPIO
    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        printf("Failed to open %s\n", GPIO_CHIP);
        return 1;
    };

    // SPI
    spi_fd = open("/dev/spidev0.0", O_WRONLY);
    if (spi_fd < 0) {
        printf("Failed to open /dev/spidev0.0\n");
        gpiod_chip_close(chip);
        chip = NULL;
        return 1;
    };

    DEV_GPIO_Init();
    return 0;
};

void DEV_ModuleExit() {
    if (spi_fd >= 0) {
        close(spi_fd);
        spi_fd = -1;
    };
    if (chip) {
        gpiod_chip_close(chip);
        chip = NULL;
    };
    exit(0);
};
