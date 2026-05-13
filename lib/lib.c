#include "lib.h"
#include <gpiod.h> // gpio
#include <fcntl.h> // spi open
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define GPIO_CHIP "/dev/gpiochip0"

static struct gpiod_chip *chip;

static int spi_fd = -1;

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

UBYTE DEV_Digital_Read(UWORD Pin) {
    if (!chip) {
        printf("DEV_Digital_Read: chip not initialized\n");
        return 0;
    };

    struct gpiod_line *line = gpiod_chip_get_line(chip, Pin);
    if (!line) {
        printf("DEV_Digital_Read: failed to get line %u\n", Pin);
        return 0;
    };

    int v = gpiod_line_get_value(line);
    if (v < 0) {
        printf("DEV_Digital_Read: failed to read value from line %u\n", Pin);
        return 0;
    };

    return (UBYTE)v;
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

    // Buttons
    DEV_GPIO_Mode(KEY_UP_PIN, 0);
    DEV_GPIO_Mode(KEY_DOWN_PIN, 0);
    DEV_GPIO_Mode(KEY_LEFT_PIN, 0);
    DEV_GPIO_Mode(KEY_RIGHT_PIN, 0);
    DEV_GPIO_Mode(KEY_PRESS_PIN, 0);
    DEV_GPIO_Mode(KEY1_PIN, 0);
    DEV_GPIO_Mode(KEY2_PIN, 0);
    DEV_GPIO_Mode(KEY3_PIN, 0);

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

    uint8_t spi_mode = SPI_MODE_0;
    uint8_t spi_bits = 8;
    uint32_t spi_speed = 8000000;
    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode) < 0 ||
        ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits) < 0 ||
        ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0) {
        printf("Failed to configure SPI!\n");
        close(spi_fd);
        spi_fd = -1;
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
